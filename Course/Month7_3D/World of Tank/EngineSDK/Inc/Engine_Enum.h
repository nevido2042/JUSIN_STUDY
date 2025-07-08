#pragma once

namespace Engine
{
    enum class COLLIDER { AABB, OBB, SPHERE, END };
    enum class MODEL { NONANIM, ANIM };
	enum class D3DTS { VIEW, PROJ, END };
	enum class RENDERGROUP{ RG_PRIORITY = 0, RG_SHADOW, RG_NONBLEND, RG_NONLIGHT, RG_BLEND_FIRST, RG_BLEND, RG_UI, RG_END };
	enum class PROTOTYPE { TYPE_GAMEOBJECT, TYPE_COMPONENT };
	enum class STATE { RIGHT, UP, LOOK, POSITION, END };
	enum class DIMK { LBUTTON, RBUTTON, WHEEL, END };
	enum class DIMM { X, Y, WHEEL, END };
	enum class NETWORK_STATUS { CONNECTED, WARNING, DISCONNECTED };
    enum class EMISSION_SHAPE { DROP, SPREAD, END };
#pragma region aiTextureType
    enum aiTextureType {
        /** Dummy value.
         *
         *  No texture, but the value to be used as 'texture semantic'
         *  (#aiMaterialProperty::mSemantic) for all material properties
         *  *not* related to textures.
         */
        aiTextureType_NONE = 0,

        /** LEGACY API MATERIALS
         * Legacy refers to materials which
         * Were originally implemented in the specifications around 2000.
         * These must never be removed, as most engines support them.
         */

         /** The texture is combined with the result of the diffuse
          *  lighting equation.
          *  OR
          *  PBR Specular/Glossiness
          */
        aiTextureType_DIFFUSE = 1,

        /** The texture is combined with the result of the specular
         *  lighting equation.
         *  OR
         *  PBR Specular/Glossiness
         */
        aiTextureType_SPECULAR = 2,

        /** The texture is combined with the result of the ambient
         *  lighting equation.
         */
        aiTextureType_AMBIENT = 3,

        /** The texture is added to the result of the lighting
         *  calculation. It isn't influenced by incoming light.
         */
        aiTextureType_EMISSIVE = 4,

        /** The texture is a height map.
         *
         *  By convention, higher gray-scale values stand for
         *  higher elevations from the base height.
         */
        aiTextureType_HEIGHT = 5,

        /** The texture is a (tangent space) normal-map.
         *
         *  Again, there are several conventions for tangent-space
         *  normal maps. Assimp does (intentionally) not
         *  distinguish here.
         */
        aiTextureType_NORMALS = 6,

        /** The texture defines the glossiness of the material.
         *
         *  The glossiness is in fact the exponent of the specular
         *  (phong) lighting equation. Usually there is a conversion
         *  function defined to map the linear color values in the
         *  texture to a suitable exponent. Have fun.
        */
        aiTextureType_SHININESS = 7,

        /** The texture defines per-pixel opacity.
         *
         *  Usually 'white' means opaque and 'black' means
         *  'transparency'. Or quite the opposite. Have fun.
        */
        aiTextureType_OPACITY = 8,

        /** Displacement texture
         *
         *  The exact purpose and format is application-dependent.
         *  Higher color values stand for higher vertex displacements.
        */
        aiTextureType_DISPLACEMENT = 9,

        /** Lightmap texture (aka Ambient Occlusion)
         *
         *  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
         *  covered by this material property. The texture contains a
         *  scaling value for the final color value of a pixel. Its
         *  intensity is not affected by incoming light.
        */
        aiTextureType_LIGHTMAP = 10,

        /** Reflection texture
         *
         * Contains the color of a perfect mirror reflection.
         * Rarely used, almost never for real-time applications.
        */
        aiTextureType_REFLECTION = 11,

        /** PBR Materials
         * PBR definitions from maya and other modelling packages now use this standard.
         * This was originally introduced around 2012.
         * Support for this is in game engines like Godot, Unreal or Unity3D.
         * Modelling packages which use this are very common now.
         */

        aiTextureType_BASE_COLOR = 12,
        aiTextureType_NORMAL_CAMERA = 13,
        aiTextureType_EMISSION_COLOR = 14,
        aiTextureType_METALNESS = 15,
        aiTextureType_DIFFUSE_ROUGHNESS = 16,
        aiTextureType_AMBIENT_OCCLUSION = 17,

        /** Unknown texture
         *
         *  A texture reference that does not match any of the definitions
         *  above is considered to be 'unknown'. It is still imported,
         *  but is excluded from any further post-processing.
        */
        aiTextureType_UNKNOWN = 18,

        /** PBR Material Modifiers
        * Some modern renderers have further PBR modifiers that may be overlaid
        * on top of the 'base' PBR materials for additional realism.
        * These use multiple texture maps, so only the base type is directly defined
        */

        /** Sheen
        * Generally used to simulate textiles that are covered in a layer of microfibers
        * eg velvet
        * https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_materials_sheen
        */
        aiTextureType_SHEEN = 19,

        /** Clearcoat
        * Simulates a layer of 'polish' or 'lacquer' layered on top of a PBR substrate
        * https://autodesk.github.io/standard-surface/#closures/coating
        * https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_materials_clearcoat
        */
        aiTextureType_CLEARCOAT = 20,

        /** Transmission
        * Simulates transmission through the surface
        * May include further information such as wall thickness
        */
        aiTextureType_TRANSMISSION = 21,

        /**
         * Maya material declarations
         */
        aiTextureType_MAYA_BASE = 22,
        aiTextureType_MAYA_SPECULAR = 23,
        aiTextureType_MAYA_SPECULAR_COLOR = 24,
        aiTextureType_MAYA_SPECULAR_ROUGHNESS = 25,

        /** Anisotropy
        * Simulates a surface with directional properties
         */
        aiTextureType_ANISOTROPY = 26,

        /**
         * gltf material declarations
         * Refs: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#metallic-roughness-material
         *           "textures for metalness and roughness properties are packed together in a single
         *           texture called metallicRoughnessTexture. Its green channel contains roughness
         *           values and its blue channel contains metalness values..."
         *       https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#_material_pbrmetallicroughness_metallicroughnesstexture
         *           "The metalness values are sampled from the B channel. The roughness values are
         *           sampled from the G channel..."
         */
        aiTextureType_GLTF_METALLIC_ROUGHNESS = 27,

#ifndef SWIG
        _aiTextureType_Force32Bit = INT_MAX
#endif
    };
#pragma endregion

}