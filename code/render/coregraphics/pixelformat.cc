//------------------------------------------------------------------------------
//  pixelformat.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/pixelformat.h"

namespace CoreGraphics
{

//------------------------------------------------------------------------------
/**
    Convert a pixel format string into a pixel format code.
*/
PixelFormat::Code
PixelFormat::FromString(const Util::String& str)
{
    if (str == "X8R8G8B8") return X8R8G8B8;
    else if (str == "R8G8B8") return R8G8B8;
    else if (str == "A8R8G8B8") return A8R8G8B8;
    else if (str == "R5G6B5") return R5G6B5;
    else if (str == "A1R5G5B5") return A1R5G5B5;
    else if (str == "A4R4G4B4") return A4R4G4B4;
    else if (str == "DXT1") return DXT1;
    else if (str == "DXT3") return DXT3;
    else if (str == "DXT5") return DXT5;
    else if (str == "R16F") return R16F;
    else if (str == "G16R16F") return G16R16F;
    else if (str == "A16B16G16R16F") return A16B16G16R16F;
    else if (str == "R32F") return R32F;         
    else if (str == "G32R32F") return G32R32F;      
    else if (str == "A32B32G32R32F") return A32B32G32R32F;
    else if (str == "A8") return A8;
    else if (str == "X2R10G10B10") return X2R10G10B10;
    else if (str == "A2R10G10B10") return A2R10G10B10;
    else if (str == "G16R16") return G16R16;
    else if (str == "D24S8") return D24S8;
    else if (str == "D24X8") return D24X8;

    // Xbox360 pixel-formats
    else if (str == "DXN") return DXN;
    else if (str == "LINDXN") return LINDXN;
    else if (str == "LINDXT1") return LINDXT1;
    else if (str == "LINDXT3") return LINDXT3;
    else if (str == "LINDXT5") return LINDXT5;
    else if (str == "LINA8R8G8B8") return LINA8R8G8B8;
    else if (str == "LINX8R8G8B8") return LINX8R8G8B8;
    else if (str == "EDG16R16") return EDG16R16;
    else if (str == "CTX1") return CTX1;
    else if (str == "LINCTX1") return LINCTX1;
    else if (str == "D24FS8") return D24FS8;

    // Wii pixel-formats
    else if (str == "I4") return I4;
    else if (str == "I8") return I8;
    else if (str == "IA4") return IA4;
    else if (str == "IA8") return IA8;
    else if (str == "Z8") return Z8;
    else if (str == "Z16") return Z16;
    else if (str == "Z24X8") return Z24X8;
    else if (str == "R4") return R4;
    else if (str == "RA4") return RA4;
    else if (str == "RA8") return RA8;
    else if (str == "R8") return R8;
    else if (str == "G8") return G8;
    else if (str == "B8") return B8;
    else if (str == "RG8") return RG8;
    else if (str == "GB8") return GB8;
    else if (str == "Z4") return Z4;
    else if (str == "Z8M") return Z8M;
    else if (str == "Z8L") return Z8L;
    else if (str == "Z16L") return Z16L;
    else if (str == "RGBA8") return RGBA8;
    else if (str == "RGB5A3") return RGB5A3;
    else if (str == "RGB565") return RGB565;
    else if (str == "CMPR") return CMPR;

    // PS3 pixel-formats
    else if (str == "X1R5G5B5_Z1R5G5B5")    return X1R5G5B5_Z1R5G5B5;
    else if (str == "X1R5G5B5_O1R5G5B5")    return X1R5G5B5_O1R5G5B5;
    else if (str == "X8R8G8B8_Z8R8G8B8")    return X8R8G8B8_Z8R8G8B8;
    else if (str == "G8B8")                 return G8B8;
    else if (str == "X8B8G8R8_Z8B8G8R8")    return X8B8G8R8_Z8B8G8R8;
    else if (str == "X8B8G8R8_O8B8G8R8")    return X8B8G8R8_O8B8G8R8;
    else if (str == "A8B8G8R8")             return A8B8G8R8;
    else if (str == "R6G5B5")               return R6G5B5;
    else if (str == "DEPTH24D8")            return DEPTH24D8;
    else if (str == "DEPTH24D8F")           return DEPTH24D8F;
    else if (str == "DEPTH16")              return DEPTH16;
    else if (str == "DEPTH16F")             return DEPTH16F;
    else if (str == "X16")                  return X16;
    else if (str == "COMPRESSED_HILO8")     return COMPRESSED_HILO8;
    else if (str == "COMPRESSED_HILO_S8")   return COMPRESSED_HILO_S8;
    else if (str == "COMPRESSED_B8R8_G8R8") return COMPRESSED_B8R8_G8R8;
    else if (str == "COMPRESSED_R8B8_R8G8") return COMPRESSED_R8B8_R8G8;
    else
    {
        n_error("Invalid pixel format string '%s'!", str.AsCharPtr());
        return InvalidPixelFormat;
    }
}

//------------------------------------------------------------------------------
/**
    Convert pixel format code into a string.
*/
Util::String
PixelFormat::ToString(PixelFormat::Code code)
{
    switch (code)
    {
        case X8R8G8B8:      return "X8R8G8B8";
        case R8G8B8:        return "R8G8B8";
        case A8R8G8B8:      return "A8R8G8B8";
        case R5G6B5:        return "R5G6B5";
        case A1R5G5B5:      return "A1R5G5B5";
        case A4R4G4B4:      return "A4R4G4B4";
        case DXT1:          return "DXT1";
        case DXT3:          return "DXT3";
        case DXT5:          return "DXT5";
        case R16F:          return "R16F";
        case G16R16F:       return "G16R16F";
        case A16B16G16R16F: return "A16B16G16R16F";
        case R32F:          return "R32F";
        case G32R32F:       return "G32R32F";
        case A32B32G32R32F: return "A32B32G32R32F";
        case A8:            return "A8";
        case X2R10G10B10:   return "X2R10G10B10";
        case A2R10G10B10:   return "A2R10G10B10";
        case G16R16:        return "G16R16";
        case D24X8:         return "D24X8";
        case D24S8:         return "D24S8";

        // Xbox360 pixel-formats
        case DXN:               return "DXN";
        case LINDXN:            return "LINDXN";
        case LINDXT1:           return "LINDXT1";
        case LINDXT3:           return "LINDXT3";
        case LINDXT5:           return "LINDXT5";
        case LINA8R8G8B8:       return "LINA8R8G8B8";
        case LINX8R8G8B8:       return "LINX8R8G8B8";
        case LINA16B16G16R16F:  return "LINA16B16G16R16F";
        case EDG16R16:          return "EDG16R16";
        case CTX1:              return "CTX1";
        case LINCTX1:           return "LINCTX1";
        case D24FS8:            return "D24FS8";

        // Wii pixel-formats
        case I4:            return "I4";
        case I8:            return "I8";
        case IA4:           return "IA4";
        case IA8:           return "IA8";
        case Z8:            return "Z8";
        case Z16:           return "Z16";
        case Z24X8:         return "Z24X8";
        case R4:            return "R4";
        case RA4:           return "RA4";
        case RA8:           return "RA8";
        case R8:            return "R8";
        case G8:            return "G8";
        case B8:            return "B8";
        case RG8:           return "RG8";
        case GB8:           return "GB8";
        case Z4:            return "Z4";
        case Z8M:           return "Z8M";
        case Z8L:           return "Z8L";
        case Z16L:          return "Z16L";
        case RGBA8:         return "RGBA8";
        case RGB5A3:        return "RGB5A3";
        case RGB565:        return "RGB565";
        case CMPR:          return "CMPR";

        // PS3 pixel-formats
        case X1R5G5B5_Z1R5G5B5:     return "X1R5G5B5_Z1R5G5B5";
        case X1R5G5B5_O1R5G5B5:     return "X1R5G5B5_O1R5G5B5";
        case X8R8G8B8_Z8R8G8B8:     return "X8R8G8B8_Z8R8G8B8";
        case G8B8:                  return "G8B8";
        case X8B8G8R8_Z8B8G8R8:     return "X8B8G8R8_Z8B8G8R8";
        case X8B8G8R8_O8B8G8R8:     return "X8B8G8R8_O8B8G8R8";
        case A8B8G8R8:              return "A8B8G8R8";
        case R6G5B5:                return "R6G5B5";
        case DEPTH24D8:             return "DEPTH24D8";
        case DEPTH24D8F:            return "DEPTH24D8F";
        case DEPTH16:               return "DEPTH16";
        case DEPTH16F:              return "DEPTH16F";
        case X16:                   return "X16";
        case COMPRESSED_HILO8:      return "COMPRESSED_HILO8";
        case COMPRESSED_HILO_S8:    return "COMPRESSED_HILO_S8";
        case COMPRESSED_B8R8_G8R8:  return "COMPRESSED_B8R8_G8R8";
        case COMPRESSED_R8B8_R8G8:  return "COMPRESSED_R8B8_R8G8";

        default:
            n_error("Invalid pixel format code");
            return "";
    }
}

} // namespace CoreGraphics
