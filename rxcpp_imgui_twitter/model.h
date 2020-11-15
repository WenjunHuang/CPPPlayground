//
// Created by rick on 2020/11/15.
//

#pragma once
#include "json.hpp"
#include <map>
#include <string>
#include <unordered_set>
#include <vector>
#include <range/v3/action.hpp>
#include <rx.hpp>
#include "util.h"
using namespace std;
using namespace ::util;
using namespace nlohmann;

namespace model {
    inline string tweetText(const json& tweet) {
        if (!!tweet.count("extended_tweet")) {
            auto ex = tweet["extended_tweet"];
            if (!!ex.count("full_text") && ex["full_text"].is_string()) {
                return ex["full_text"];
            }
        }
        if (!!tweet.count("text") && tweet["text"].is_string()) {
            return tweet["text"];
        }
        return {};
    }

    inline vector<string> splitWords(const string& text) {
        static const unordered_set<string> ignoredWords{
            // added
            "rt", "like", "just", "tomorrow", "new", "year", "month", "day",
            "today", "make", "let", "want", "did", "going", "good", "really",
            "know", "people", "got", "life", "need", "say", "doing", "great",
            "right", "time", "best", "happy", "stop", "think", "world", "watch",
            "gonna", "remember", "way", "better", "team", "check", "feel",
            "talk", "hurry", "look", "live", "home", "game", "run", "i'm",
            "you're", "person", "house", "real", "thing", "lol", "has",
            "things", "that's", "thats", "fine", "i've", "you've", "y'all",
            "didn't", "said", "come", "coming", "haven't", "won't", "can't",
            "don't", "shouldn't", "hasn't", "doesn't", "i'd", "it's", "i'll",
            "what's", "we're", "you'll", "let's'", "lets", "vs", "win", "says",
            "tell", "follow", "comes", "look", "looks", "post", "join", "add",
            "does", "went", "sure", "wait", "seen", "told", "yes", "video",
            "lot", "looks", "long", "e280a6", "\xe2\x80\xa6",
            // http://xpo6.com/list-of-english-stop-words/
            "a", "about", "above", "above", "across", "after", "afterwards",
            "again", "against", "all", "almost", "alone", "along", "already",
            "also", "although", "always", "am", "among", "amongst", "amoungst",
            "amount", "an", "and", "another", "any", "anyhow", "anyone",
            "anything", "anyway", "anywhere", "are", "around", "as", "at",
            "back", "be", "became", "because", "become", "becomes", "becoming",
            "been", "before", "beforehand", "behind", "being", "below",
            "beside", "besides", "between", "beyond", "bill", "both", "bottom",
            "but", "by", "call", "can", "cannot", "cant", "co", "con", "could",
            "couldnt", "cry", "de", "describe", "detail", "do", "done", "down",
            "due", "during", "each", "eg", "eight", "either", "eleven", "else",
            "elsewhere", "empty", "enough", "etc", "even", "ever", "every",
            "everyone", "everything", "everywhere", "except", "few", "fifteen",
            "fify", "fill", "find", "fire", "first", "five", "for", "former",
            "formerly", "forty", "found", "four", "from", "front", "full",
            "further", "get", "give", "go", "had", "has", "hasnt", "have", "he",
            "hence", "her", "here", "hereafter", "hereby", "herein", "hereupon",
            "hers", "herself", "him", "himself", "his", "how", "however",
            "hundred", "ie", "if", "in", "inc", "indeed", "interest", "into",
            "is", "it", "its", "itself", "keep", "last", "latter", "latterly",
            "least", "less", "ltd", "made", "many", "may", "me", "meanwhile",
            "might", "mill", "mine", "more", "moreover", "most", "mostly",
            "move", "much", "must", "my", "myself", "name", "namely", "neither",
            "never", "nevertheless", "next", "nine", "no", "nobody", "none",
            "noone", "nor", "not", "nothing", "now", "nowhere", "of", "off",
            "often", "on", "once", "one", "only", "onto", "or", "other",
            "others", "otherwise", "our", "ours", "ourselves", "out", "over",
            "own", "part", "per", "perhaps", "please", "put", "rather", "re",
            "same", "see", "seem", "seemed", "seeming", "seems", "serious",
            "several", "she", "should", "show", "side", "since", "sincere",
            "six", "sixty", "so", "some", "somehow", "someone", "something",
            "sometime", "sometimes", "somewhere", "still", "such", "system",
            "take", "ten", "than", "that", "the", "their", "them", "themselves",
            "then", "thence", "there", "thereafter", "thereby", "therefore",
            "therein", "thereupon", "these", "they", "thickv", "thin", "third",
            "this", "those", "though", "three", "through", "throughout", "thru",
            "thus", "to", "together", "too", "top", "toward", "towards",
            "twelve", "twenty", "two", "un", "under", "until", "up", "upon",
            "us", "very", "via", "was", "we", "well", "were", "what",
            "whatever", "when", "whence", "whenever", "where", "whereafter",
            "whereas", "whereby", "wherein", "whereupon", "wherever", "whether",
            "which", "while", "whither", "who", "whoever", "whole", "whom",
            "whose", "why", "will", "with", "within", "without", "would", "yet",
            "you", "your", "yours", "yourself", "yourselves", "the"};

        static const string delimiters = R"(\s+)";
        auto words = split(text,delimiters,::util::Split::RemoveDelimiter);


        static const regex ignore(R"((\xe2\x80\xa6)|(&[\w]+;)|((http|ftp|https)://[\w-]+(.[\w-]+)+([\w.,@?^=%&:/~+#-]*[\w@?^=%&/~+#-])?))");
        static const regex expletives(R"(\x66\x75\x63\x6B|\x73\x68\x69\x74|\x64\x61\x6D\x6E)");

        for (auto& word: words) {
            while (!word.empty() && (word.front() == '.' || word.front() == '(' || word.front() == '\'' || word.front() == '\"')) word.erase(word.begin());
            while (!word.empty() && (word.back() == ':' || word.back() == ',' || word.back() == ')' || word.back() == '\'' || word.back() == '\"')) word.resize(word.size() - 1);
            if (!word.empty() && word.front() == '@') continue;
            word = regex_replace(tolower(word), ignore, "");
            if (!word.empty() && word.front() != '#') {
                while (!word.empty() && ispunct(word.front())) word.erase(word.begin());
                while (!word.empty() && ispunct(word.back())) word.resize(word.size() - 1);
            }
            word = regex_replace(word, expletives, "<expletive>");
        }

        words.erase(std::remove_if(words.begin(), words.end(), [=](const string& w){
          return !(w.size() > 2 && ignoredWords.find(w) == ignoredWords.end());
        }), words.end());

        words |=
            ranges::actions::sort |
            ranges::actions::unique;

        return words;
    }

    struct TimeRange{
        using timestamp = milliseconds ;

        timestamp begin;
        timestamp end;
    };

    bool operator<(const TimeRange& lhs,const TimeRange& rhs){
        return lhs.begin < rhs.begin && lhs.end < rhs.end;
    }

    using WordCountMap = unordered_map<string,int>;

    struct Tweet {
        struct shared {
            json tweet;
            vector<string> words;
            shared() {}
            explicit shared(const json& t):tweet{t},words(splitWords(tweetText(tweet))) {
            }
        };
    };

    struct TweetGroup {
        deque<Tweet> tweets;
        WordCountMap words;
        int positive = 0;
        int negative = 0;
        int toxic = 0;
    };

    struct Perspective{
        float toxicity;
        float spam;
        float inflammatory;
    };

    struct Model {
        struct shared{
            string url;
            rxsc::scheduler::clock_type::time_point timestamp;
            int total = 0;
            deque<TimeRange> groups;
            std::map<TimeRange,shared_ptr<TweetGroup>> groupedTweets;
            seconds tweetsStart;
            deque<int> tweetsperminute;
            deque<Tweet> tweets;
            WordCountMap allwords;
            WordCountMap positivewords;
            WordCountMap negativewords;
            WordCountMap toxicwords;
            unordered_map<string,string> sentiment;
            unordered_map<string,Perspective> perspective;
        };
    };
} // namespace model