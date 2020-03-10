//
// Created by rick on 2020/3/11.
//
#include <string_view>
#include <iostream>
#include <sstream>

int main() {
    std::string_view text(
        "Qlfh$#Li#|rx#duh#uhdglqj#wklv#|rx#pxvw#kdyh#zrunhg#rxw#krz#wr#ghfu|sw#"
        "lw1#Wklv#lv#rxu#lvvxh#wudfnhu#ghvljqhg#iru#djloh#whdpv1#Lw#lv#iuhh#"
        "iru#xs#wr#6#xvhuv#lq#Forxg#dqg#iru#43#xvhuv#lq#Vwdqgdorqh/"
        "#vr#li#|rx#zdqw#wr#jlyh#lw#d#jr#lq#|rxu#whdp#wkhq#zh#wrwdoo|#"
        "uhfrpphqg#lw1#|rx#kdyh#ilqlvkhg#wkh#iluvw#Txhvw/"
        "#qrz#lw“v#wlph#wr#uhghhp#|rxu#iluvw#sul}h1#Wkh#frgh#iru#wkh#iluvw#"
        "txhvw#lv#‟WkhGulyhWrGhyhors†1#Jr#wr#wkh#Txhvw#Sdjh#dqg#xvh#wkh#frgh#"
        "wr#fodlp#|rxu#sul}h1#kwwsv=22zzz1mhweudlqv1frp2surpr2txhvw2");
    std::string_view goodLuck("Good luck!");
    std::string_view encode("Jrrg#oxfn$");
    std::stringstream out;

//    for (auto i = 0;i < 10;i++) {
//        std::cout << encode[i] - goodLuck[i] << '\n';
//    }
    for (char c:text) {
        out << (char)(c - 3);
    }
    std::cout << out.str() << "\n";
//    for (char c : text) {
//        auto index = encode.find(c);
//        if (index != std::string_view::npos) {
//            out << goodLuck[index];
//        } else {
//            out << c;
//        }
//    }
//
}