/*
 *      Copyright (C) 2017 Team Kodi
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "guilib/Geometry.h"
#include "cores/IPlayer.h"

#include "libavutil/pixfmt.h"

#include <stdint.h>

namespace KODI
{
namespace RETRO
{
  class CRPBaseRenderer
  {
  public:
    CRPBaseRenderer();
    virtual ~CRPBaseRenderer();

    // Player functions
    virtual bool Configure(AVPixelFormat format, unsigned int width, unsigned int height, unsigned int orientation) = 0;
    virtual void AddFrame(const uint8_t* data, unsigned int size) = 0;
    virtual void Flush() = 0;
    virtual void RenderUpdate(bool clear, unsigned int alpha) = 0;
    virtual void Deinitialize() = 0;

    // Feature support
    virtual bool Supports(ERENDERFEATURE feature) const = 0;
    virtual bool Supports(ESCALINGMETHOD method) const = 0;

    /*!
     * \brief Get video rectangle and view window
     *
     * \param source is original size of the video
     * \param dest is the target rendering area honoring aspect ratio of source
     * \param view is the entire target rendering area for the video (including black bars)
     */
    void GetVideoRect(CRect &source, CRect &dest, CRect &view);
    float GetAspectRatio() const;

    ESCALINGMETHOD GetScalingMethod() const { return m_scalingMethod; }
    void SetScalingMethod(ESCALINGMETHOD method);

    ViewMode GetViewMode() const { return m_viewMode; }
    void SetViewMode(ViewMode viewMode);

    bool IsNonLinearStretch() const { return m_bNonLinearStretch; }

  protected:
    /*!
     * \brief Call this from the base class to initialize render settings
     * to current game settings values
     */
    void LoadGameSettings();

    void CalculateFrameAspectRatio(unsigned int desired_width, unsigned int desired_height);
    void CalcNormalRenderRect(float offsetX, float offsetY, float width, float height, float inputFrameRatio, float zoomAmount);
    virtual void ManageRenderArea();
    virtual void ReorderDrawPoints();
    void MarkDirty();
    float GetAllowedErrorInAspect() const;

    // Stream properties
    AVPixelFormat m_format = AV_PIX_FMT_NONE;
    unsigned int m_sourceWidth = 0;
    unsigned int m_sourceHeight = 0;
    unsigned int m_renderOrientation = 0; // Degrees counter-clockwise

    /*!
     * \brief Orientation of the previous frame
     *
     * For drawing the texture with glVertex4f (holds all 4 corner points of the
     * destination rect with correct orientation based on m_renderOrientation.
     */
    unsigned int m_oldRenderOrientation = 0;

    // Rendering properties
    ESCALINGMETHOD m_scalingMethod = VS_SCALINGMETHOD_NEAREST;
    ViewMode m_viewMode = ViewModeNormal;
    float m_pixelRatio = 1.0f;
    float m_zoomAmount = 1.0f;
    bool m_bNonLinearStretch = false;

    // Geometry properties
    float m_sourceFrameRatio = 1.0f;

    CPoint m_rotatedDestCoords[4];

    CRect m_destRect;
    CRect m_oldDestRect; // destrect of the previous frame
    CRect m_sourceRect;
    CRect m_viewRect;

  };
}
}