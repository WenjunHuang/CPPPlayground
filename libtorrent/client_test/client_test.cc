//
// Created by rick on 2020/4/5.
//
#include <algorithm>
#include <cstdlib>
#include <deque>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/announce_entry.hpp>
#include <libtorrent/bdecode.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/config.hpp>
#include <libtorrent/disk_interface.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/identify_client.hpp>
#include <libtorrent/ip_filter.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/peer_info.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/string_view.hpp>
#include <libtorrent/time.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/write_resume_data.hpp>
#include <regex>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <utility>

#include "session_view.h"
#include "torrent_view.h"

#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#include <conio.h>

using namespace lt;
using std::stoi;
using std::chrono::duration_cast;

bool sleep_and_input(int* c, lt::time_duration const sleep) {
    for (int i = 0; i < 2; ++i) {
        if (_kbhit()) {
            *c = _getch();
            return true;
        }
        std::this_thread::sleep_for(sleep / 2);
    }
    return false;
}
#else
#endif

bool print_trackers         = false;
bool print_peers            = false;
bool print_connecting_peers = false;
bool print_log              = false;
bool print_downloads        = false;
bool print_matrix           = false;
bool print_file_progress    = false;
bool show_pad_files         = false;
bool show_dht_status        = false;
bool sequential_download    = false;

bool print_ip         = true;
bool print_local_ip   = false;
bool print_timers     = false;
bool print_block      = false;
bool print_fails      = false;
bool print_send_bufs  = true;
bool print_disk_stats = false;

int num_outstanding_resume_data = 0;

std::vector<lt::dht_lookup> dht_active_requests;
std::vector<lt::dht_routing_bucket> dht_routing_table;

std::string to_hex(lt::sha1_hash const& s) {
    std::stringstream ret;
    ret << s;
    return ret.str();
}

bool load_file(std::string const& filename, std::vector<char>& v,
               int limit = 8000000) {
    std::fstream f{filename, std::ios_base::in | std::ios_base::binary};
    f.seekg(0, std::ios_base::end);

    auto const s = f.tellg();
    if (s > limit || s < 0)
        return false;

    f.seekg(0, std::ios_base::beg);
    v.resize(static_cast<std::size_t>(s));
    if (s == std::fstream::pos_type(0))
        return !f.fail();
    f.read(v.data(), v.size());
    return !f.fail();
}

bool is_absolute_path(std::string const& f) {
    std::filesystem::path p(f);
    return p.is_absolute();
}

std::string path_append(std::string const& lhs, std::string const& rhs) {
    std::filesystem::path p{lhs};
    p /= rhs;
    return p.lexically_normal().string();
}

std::string make_absolute_path(std::string const& p) {
    if (is_absolute_path(p))
        return p;

    std::string ret;

#ifdef _WIN32
    char* cwd = ::_getcwd(nullptr, 0);
    ret       = path_append(cwd, p);
    std::free(cwd);
#else
#endif

    return ret;
}

std::string print_endpoint(lt::tcp::endpoint const& ep) {
    lt::error_code ec;
    char buf[200];

    lt::address const& addr = ep.address();
    if (addr.is_v6())
        std::snprintf(buf, sizeof(buf), "[%s]:%d", addr.to_string(ec).c_str(),
                      ep.port());
    else
        std::snprintf(buf, sizeof(buf), "%s:%d", addr.to_string(ec).c_str(),
                      ep.port());

    return buf;
}

int peer_index(lt::tcp::endpoint addr,
               std::vector<lt::peer_info> const& peers) {
    auto i = std::find_if(
        peers.begin(), peers.end(),
        [&addr](lt::peer_info const& pi) { return pi.ip == addr; });

    if (i == peers.end())
        return -1;

    return int(i - peers.begin());
}

// returns the number of lines printed
int print_peer_info(std::string& out, std::vector<lt::peer_info> const& peers,
                    int max_lines) {
    int pos = 0;
    if (print_ip)
        out += "IP                             ";
    if (print_local_ip)
        out += "local IP                       ";

    out += "progress        down     (total | peak   )  up      (total | peak  "
           " ) sent-req tmo bsy rcv flags         dn  up  source  ";
    if (print_fails)
        out += "fail hshf ";
    if (print_send_bufs)
        out += "rq sndb (recvb |alloc | wmrk ) q-bytes ";
    if (print_timers)
        out += "inactive wait timeout q-time ";
    out += "  v disk ^    rtt  ";
    if (print_block)
        out += "block-progress ";

    out += "client \x1b[K\n";
    ++pos;

    std::array<char, 500> str{};
    for (auto const& i : peers) {
        if (i.flags & (lt::peer_info::handshake | lt::peer_info::connecting) &&
            !print_connecting_peers) {
            continue;
        }

        if (print_ip) {
            std::snprintf(
                str.data(),str.max_size(), "%-30s ",
                (print_endpoint(i.ip) +
                 (i.flags & lt::peer_info::utp_socket ? " [uTP]" : "") +
                 (i.flags & lt::peer_info::i2p_socket ? " [i2p]" : ""))
                    .c_str());
            out += str.data();
        }

        if (print_local_ip) {
            std::snprintf(str.data(),str.max_size(), "%-30s ",
                          print_endpoint(i.local_endpoint).c_str());
            out += str.data();
        }

        std::array<char,10> temp{};
        std::snprintf(temp.data(),temp.max_size(), "%d/%d", i.download_queue_length,
                      i.target_dl_queue_length);

        std::array<char,10> peer_progress{};
        std::snprintf(peer_progress.data(),peer_progress.max_size(),"%.1f%%",
            i.progress_ppm / 10000.f);
        std::snprintf(str.data(),str.max_size(),
                      "%s %s%s (%s|%s) %s%s (%s|%s) %s%7s %4d%4d%4d %s%s%s%s%s%s%s%s%s%s%s%s%s %s%s%s %s%s%s %s%s%s%s%s%s ",
                      progress_bar
            )
    }
}