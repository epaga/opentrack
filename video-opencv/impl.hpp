/* Copyright (c) 2019 Stanislaw Halik <sthalik@misaki.pl>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 */

#pragma once

#include "video/camera.hpp"
#include "settings.hpp"
#include <optional>
#include <opencv2/videoio.hpp>

namespace opencv_camera_impl {

using namespace video::impl;

struct metadata : camera_
{
    metadata();
    std::vector<QString> camera_names() const override;
    std::unique_ptr<camera> make_camera(const QString& name) override;
    bool can_show_dialog(const QString& camera_name) override;
    bool show_dialog(const QString& camera_name) override;
};

struct cam final : camera
{
static constexpr int video_capture_backend =
#ifdef _WIN32
    cv::CAP_DSHOW;
#elif !defined __APPLE__
    cv::CAP_V4L2;
#else
    cv::CAP_ANY;
#endif

    cam(int idx);
    ~cam() override;

    bool start(info& args) override;
    void stop() override;
    bool is_open() override;
    std::tuple<const frame&, bool> get_frame() override;
    bool show_dialog() override;
    void set_exposure(exposure_preset e);

    bool get_frame_();

    dshow_camera_settings s;

    std::optional<cv::VideoCapture> cap;
    cv::Mat mat;
    frame frame_;
    int idx = -1, writes = 0;
    exposure_preset exposure = exposure_preset::ignored;
};

} // ns opencv_camera_impl
