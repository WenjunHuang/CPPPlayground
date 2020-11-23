//
// Created by rick on 2020/11/22.
//

#pragma once
#include <imgui.h>
#include <rxcpp/rx.hpp>
#include <rxcpp/subjects/rx-subject.hpp>

using namespace rxcpp;
namespace rximgui{
    inline float clamp(float v,float mn,float mx){
        return (v < mn) ? mn :(v> mx)?mx:v;
    }

    inline ImVec2 clamp(const ImVec2& f, const ImVec2& mn,ImVec2 mx){
        return ImVec2(clamp(f.x,mn.x,mx.x),clamp(f.y,mn.y,mx.y));
    }

    inline rxsub::subject<int> frameBus;

    inline auto frameOut = frameBus.get_subscriber();
    inline auto sendFrame = []() {
        frameOut.on_next(1);
    };
    inline auto frames = frameBus.get_observable();
    inline schedulers::run_loop rl;

}
