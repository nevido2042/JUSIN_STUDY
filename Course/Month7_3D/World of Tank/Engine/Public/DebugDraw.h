//--------------------------------------------------------------------------------------
// File: DebugDraw.h
//
// Helpers for drawing various debug shapes using PrimitiveBatch
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//-------------------------------------------------------------------------------------

#pragma once

#include "Engine_Defines.h"

namespace DX
{
    ENGINE_DLL void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
        const DirectX::BoundingSphere& sphere,
        DirectX::FXMVECTOR color = DirectX::Colors::White);

    ENGINE_DLL void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
        const DirectX::BoundingBox& box,
        DirectX::FXMVECTOR color = DirectX::Colors::White);

    ENGINE_DLL void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
        const DirectX::BoundingOrientedBox& obb,
        DirectX::FXMVECTOR color = DirectX::Colors::White);

    ENGINE_DLL void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
        const DirectX::BoundingFrustum& frustum,
        DirectX::FXMVECTOR color = DirectX::Colors::White);

    ENGINE_DLL void XM_CALLCONV DrawGrid(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
        DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis,
        DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs,
        DirectX::GXMVECTOR color = DirectX::Colors::White);

    ENGINE_DLL void XM_CALLCONV DrawRing(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
        DirectX::FXMVECTOR origin, DirectX::FXMVECTOR majorAxis, DirectX::FXMVECTOR minorAxis,
        DirectX::GXMVECTOR color = DirectX::Colors::White);

    ENGINE_DLL void XM_CALLCONV DrawRay(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
        DirectX::FXMVECTOR origin, DirectX::FXMVECTOR direction, bool normalize = true,
        DirectX::FXMVECTOR color = DirectX::Colors::White);

    ENGINE_DLL void XM_CALLCONV DrawTriangle(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
        DirectX::FXMVECTOR pointA, DirectX::FXMVECTOR pointB, DirectX::FXMVECTOR pointC,
        DirectX::GXMVECTOR color = DirectX::Colors::White);
}