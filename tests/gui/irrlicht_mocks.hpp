/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Enhanced mock implementations for Irrlicht types to enable testing without Irrlicht
*/

#pragma once

// Prevent real Irrlicht headers from being included when using mocks
#ifdef MOCK_IRRLICHT
#define _IRR_COMPILE_WITH_IRRLICHT_H_
#define __C_IRR_COMPILE_CONFIG_H_INCLUDED__
#define __IRR_POINT_3D_H_INCLUDED__
#define __IRR_VECTOR_3D_H_INCLUDED__
#define __IRR_MATRIX_4_H_INCLUDED__
#define __IRR_AABBOX_3D_H_INCLUDED__
#define __IRR_PLANE_3D_H_INCLUDED__
#define __IRR_LINE_3D_H_INCLUDED__
#define __IRR_TRIANGLE_3D_H_INCLUDED__
#define __IRR_QUATERNION_H_INCLUDED__
#define __IRR_STRING_H_INCLUDED__
#define __IRR_IRRSTRING_H_INCLUDED__
#define __IRR_FAST_ATOF_H_INCLUDED__
#define __IRR_COLOR_H_INCLUDED__
#define __IRR_LIST_H_INCLUDED__
#define __IRR_IRRARRAY_H_INCLUDED__
#define __IRR_HEAPSORT_H_INCLUDED__
#define __IRR_ALLOCATOR_H_INCLUDED__
#define __IRR_IRRMAP_H_INCLUDED__
#define __IRR_DIMENSION2D_H_INCLUDED__
#define __IRR_RECT_H_INCLUDED__
#define __IRR_IRRMATH_H_INCLUDED__
#define __IRR_IRRTYPES_H_INCLUDED__
#define __IRR_S3DVERTEX_H_INCLUDED__
#define __IRR_SMATERIAL_H_INCLUDED__
#define __IRR_IIMAGE_H_INCLUDED__
#define __IRR_ITEXTURE_H_INCLUDED__
#define __IRR_IATTRIBUTES_H_INCLUDED__
#define __IRR_EATTRIBUTES_H_INCLUDED__
#define __IRR_EPATH_H_INCLUDED__
#define __IRR_IREADFILE_H_INCLUDED__
#define __IRR_IWRITEFILE_H_INCLUDED__
#define __IRR_IFILESYSTEM_H_INCLUDED__
#define __IRR_IXMLREADER_H_INCLUDED__
#define __IRR_IXMLWRITER_H_INCLUDED__
#define __IRR_IRRDEVICE_H_INCLUDED__
#define __IRR_IEVENTRECEIVER_H_INCLUDED__
#define __IRR_ISCENENODE_H_INCLUDED__
#define __IRR_IMESH_H_INCLUDED__
#define __IRR_IANIMATEDMESH_H_INCLUDED__
#define __IRR_IMESHSCENENODE_H_INCLUDED__
#define __IRR_IANIMATEDMESHSCENENODE_H_INCLUDED__
#define __IRR_ISCENEMANAGER_H_INCLUDED__
#define __IRR_IVIDEODRIVER_H_INCLUDED__
#define __IRR_IGUITEXT_H_INCLUDED__
#define __IRR_IGUIENVIRONMENT_H_INCLUDED__
#define __IRR_ICURSORCONTROL_H_INCLUDED__
#define __IRR_ILOGGER_H_INCLUDED__
#define __IRR_IOSOPERATOR_H_INCLUDED__
#define __IRR_IRANDOMIZER_H_INCLUDED__
#define __IRR_ITIMER_H_INCLUDED__
#define _IRRLICHT_H_INCLUDED_
#endif

#include <string>
#include <vector>
#include <memory>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Mock Irrlicht namespace and basic types
namespace irr {
    typedef unsigned int u32;
    typedef int s32;
    typedef float f32;
    
    namespace core {
        template<typename T>
        struct vector3d {
            T X, Y, Z;
            
            vector3d() : X(0), Y(0), Z(0) {}
            vector3d(T x, T y, T z) : X(x), Y(y), Z(z) {}
            
            bool operator==(const vector3d& other) const {
                return X == other.X && Y == other.Y && Z == other.Z;
            }
            
            bool operator!=(const vector3d& other) const {
                return !(*this == other);
            }
            
            void set(T x, T y, T z) {
                X = x; Y = y; Z = z;
            }
            
            vector3d<T> getInterpolated(const vector3d<T>& other, f32 factor) const {
                return vector3d<T>(X + (other.X - X) * factor,
                                  Y + (other.Y - Y) * factor,
                                  Z + (other.Z - Z) * factor);
            }
            
            vector3d<T> operator-(const vector3d<T>& other) const {
                return vector3d<T>(X - other.X, Y - other.Y, Z - other.Z);
            }
            
            vector3d<T> operator+(const vector3d<T>& other) const {
                return vector3d<T>(X + other.X, Y + other.Y, Z + other.Z);
            }
            
            vector3d<T>& operator+=(const vector3d<T>& other) {
                X += other.X; Y += other.Y; Z += other.Z;
                return *this;
            }
            
            vector3d<T>& operator-=(const vector3d<T>& other) {
                X -= other.X; Y -= other.Y; Z -= other.Z;
                return *this;
            }
            
            vector3d<T> operator*(T scalar) const {
                return vector3d<T>(X * scalar, Y * scalar, Z * scalar);
            }
            
            vector3d<T> crossProduct(const vector3d<T>& other) const {
                return vector3d<T>(Y * other.Z - Z * other.Y,
                                  Z * other.X - X * other.Z,
                                  X * other.Y - Y * other.X);
            }
            
            vector3d<T>& normalize() {
                f32 length = sqrt(X*X + Y*Y + Z*Z);
                if (length > 0) {
                    X /= length; Y /= length; Z /= length;
                }
                return *this;
            }
        };
        
        typedef vector3d<float> vector3df;
        typedef vector3d<s32> vector3di;
        
        template<typename T, typename Allocator = std::allocator<T>>
        class string : public std::basic_string<T> {
        public:
            string() = default;
            
            // Generic constructor for basic_string types
            template<typename U>
            string(const std::basic_string<U>& str) : std::basic_string<T>(str.begin(), str.end()) {}
            
            // Specializations for different types
            template<typename U = T>
            string(const char* str, typename std::enable_if<std::is_same<U, char>::value>::type* = 0) 
                : std::basic_string<T>(str) {}
                
            template<typename U = T>
            string(const wchar_t* str, typename std::enable_if<std::is_same<U, wchar_t>::value>::type* = 0) 
                : std::basic_string<T>(str) {}
                
            // Constructor for numeric types
            template<typename NumericType>
            string(NumericType value, typename std::enable_if<std::is_arithmetic<NumericType>::value>::type* = 0) {
                *this = std::to_string(value);
            }
            
            // Additional assignment and concatenation operators
            string& operator+=(const T* str) {
                std::basic_string<T>::operator+=(str);
                return *this;
            }
            
            string& operator+=(const string& other) {
                std::basic_string<T>::operator+=(other);
                return *this;
            }
            
            string& operator+=(const std::basic_string<T>& other) {
                std::basic_string<T>::operator+=(other);
                return *this;
            }
            
            // Concatenation operators
            string operator+(const T* str) const {
                string result(*this);
                result += str;
                return result;
            }
            
            string operator+(const string& other) const {
                string result(*this);
                result += other;
                return result;
            }
            
            string operator+(const std::basic_string<T>& other) const {
                string result(*this);
                result += other;
                return result;
            }
        };
        
        typedef string<char> stringc;
        typedef string<wchar_t> stringw;
        
        template<typename T>
        class irrAllocator {
        public:
            typedef T value_type;
        };
        
        template<typename T>
        struct dimension2d {
            T Width, Height;
            
            dimension2d() : Width(0), Height(0) {}
            dimension2d(T width, T height) : Width(width), Height(height) {}
        };
        
        typedef dimension2d<u32> dimension2du;
        
        template<typename T>
        struct position2d {
            T X, Y;
            
            position2d() : X(0), Y(0) {}
            position2d(T x, T y) : X(x), Y(y) {}
        };
        
        template<typename T>
        struct rect {
            T UpperLeftCorner, LowerRightCorner;
            
            rect() {}
            rect(T x1, T y1, T x2, T y2) : UpperLeftCorner(x1), LowerRightCorner(x2) {
                (void)y1; (void)y2;
            }
        };
        
        template<typename T>
        struct line3d {
            vector3d<T> start, end;
            
            line3d() {}
            line3d(const vector3d<T>& s, const vector3d<T>& e) : start(s), end(e) {}
        };
        
        class matrix4 {
        public:
            matrix4() {}
            void setRotationDegrees(const vector3df& rotation) {
                (void)rotation;
            }
            vector3df getRotationDegrees() const {
                return vector3df(0, 0, 0);
            }
            void rotateVect(vector3df& vect) const {
                (void)vect; // Mock implementation - no actual rotation
            }
            void setRotationAxisRadians(f32 angle, const vector3df& axis) {
                (void)angle; (void)axis;
            }
        };
    }
    
    namespace io {
        typedef core::string<char> path;
        
        class IFileSystem {
        public:
            virtual ~IFileSystem() = default;
        };
    }
    
    // Forward declarations
    namespace gui {
        class IGUIElement;
        class IGUIStaticText;
        class IGUIEnvironment;
        class IGUIFont;
    }
    
    namespace scene {
        class ISceneNode;
        class ICameraSceneNode;
        class IMeshSceneNode;
        class ISceneCollisionManager;
    }
    
    namespace video {
        enum E_MATERIAL_FLAG {
            EMF_LIGHTING = 0,
            EMF_ZBUFFER,
            EMF_ZWRITE_ENABLE,
            EMF_BACK_FACE_CULLING
        };
        
        enum E_DRIVER_TYPE {
            EDT_NULL = 0,
            EDT_SOFTWARE,
            EDT_BURNINGSVIDEO,
            EDT_DIRECT3D8,
            EDT_DIRECT3D9,
            EDT_OPENGL,
            EDT_COUNT
        };
        
        enum E_TEXTURE_CREATION_FLAG {
            ETCF_ALWAYS_16_BIT = 0x00000001,
            ETCF_ALWAYS_32_BIT = 0x00000002,
            ETCF_OPTIMIZED_FOR_QUALITY = 0x00000004,
            ETCF_OPTIMIZED_FOR_SPEED = 0x00000008,
            ETCF_CREATE_MIP_MAPS = 0x00000010,
            ETCF_NO_ALPHA_CHANNEL = 0x00000020,
            ETCF_ALLOW_NON_POWER_2 = 0x00000040
        };
        
        class ITexture {
        public:
            virtual ~ITexture() = default;
        };
        
        struct SMaterial {
            bool Lighting = false;
            bool NormalizeNormals = false;
            
            void setFlag(E_MATERIAL_FLAG flag, bool value) {
                (void)flag;
                (void)value;
            }
            void setTexture(u32 layer, ITexture* texture) {
                (void)layer; (void)texture;
            }
        };
        
        struct SColor {
            u32 color;
            
            SColor() : color(0) {}
            SColor(u32 a, u32 r, u32 g, u32 b) {
                color = (a << 24) | (r << 16) | (g << 8) | b;
            }
        };
        
        struct SColorf {
            f32 r, g, b, a;
            
            SColorf() : r(0), g(0), b(0), a(1) {}
            SColorf(f32 red, f32 green, f32 blue, f32 alpha = 1.0f) 
                : r(red), g(green), b(blue), a(alpha) {}
        };
        
        class IVideoDriver {
        public:
            virtual ~IVideoDriver() = default;
            virtual void setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled) {
                (void)flag; (void)enabled;
            }
            virtual ITexture* getTexture(const io::path& filename) {
                (void)filename;
                return nullptr;
            }
            virtual bool beginScene(bool backBuffer = true, bool zBuffer = true, SColor color = SColor(255, 0, 0, 0)) {
                (void)backBuffer; (void)zBuffer; (void)color;
                return true;
            }
            virtual bool endScene() { return true; }
            virtual s32 getFPS() const { return 60; }
            virtual void makeColorKeyTexture(ITexture* texture, const core::position2d<s32>& colorKeyPixel) {
                (void)texture; (void)colorKeyPixel;
            }
            virtual core::dimension2d<u32> getScreenSize() const { 
                return core::dimension2d<u32>(1920, 1080); 
            }
        };
    }
    
    namespace scene {
        class ISceneNode {
        public:
            virtual ~ISceneNode() = default;
            virtual void setPosition(const core::vector3df& pos) { (void)pos; }
            virtual void setScale(const core::vector3df& scale) { (void)scale; }
            virtual void setRotation(const core::vector3df& rotation) { (void)rotation; }
            virtual void setID(s32 id) { (void)id; }
            virtual s32 getID() const { return 0; }
            virtual core::vector3df getPosition() const { return core::vector3df(0, 0, 0); }
            virtual ISceneNode* getParent() const { return nullptr; }
            virtual void remove() {}
            virtual void setVisible(bool visible) { (void)visible; }
            virtual const wchar_t* getName() const { return L"MockNode"; }
        };
        
        class IAnimatedMesh {
        public:
            virtual ~IAnimatedMesh() = default;
        };
        
        class IAnimatedMeshSceneNode : public ISceneNode {
        public:
            virtual ~IAnimatedMeshSceneNode() = default;
            virtual void setAnimationSpeed(f32 speed) { (void)speed; }
            virtual u32 getMaterialCount() const { return 0; }
            virtual video::SMaterial& getMaterial(u32 i) {
                static video::SMaterial dummy;
                (void)i;
                return dummy;
            }
            virtual core::vector3df getRotation() const { return core::vector3df(0, 0, 0); }
            virtual void setFrameLoop(s32 begin, s32 end) { (void)begin; (void)end; }
        };
        
        class ISceneManager {
        public:
            virtual ~ISceneManager() = default;
            virtual IAnimatedMeshSceneNode* addAnimatedMeshSceneNode(IAnimatedMesh* mesh, ISceneNode* parent = nullptr, s32 id = -1) {
                (void)mesh; (void)parent; (void)id;
                return nullptr;
            }
            virtual IAnimatedMesh* getMesh(const io::path& filename) { 
                (void)filename;
                return nullptr; 
            }
            virtual ISceneNode* addSkyBoxSceneNode(
                video::ITexture* top, video::ITexture* bottom, video::ITexture* left,
                video::ITexture* right, video::ITexture* front, video::ITexture* back,
                ISceneNode* parent = nullptr, s32 id = -1) {
                (void)top; (void)bottom; (void)left; (void)right; (void)front; (void)back;
                (void)parent; (void)id;
                return nullptr;
            }
            virtual void drawAll() {}
            virtual ISceneNode* addLightSceneNode(ISceneNode* parent = nullptr,
                const core::vector3df& position = core::vector3df(0, 0, 0),
                video::SColorf color = video::SColorf(1.0f, 1.0f, 1.0f),
                f32 radius = 100.0f, s32 id = -1) {
                (void)parent; (void)position; (void)color; (void)radius; (void)id;
                return nullptr;
            }
            virtual scene::ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = nullptr,
                const core::vector3df& position = core::vector3df(0, 0, 0),
                const core::vector3df& lookat = core::vector3df(0, 0, 100),
                s32 id = -1) {
                (void)parent; (void)position; (void)lookat; (void)id;
                return nullptr;
            }
            virtual video::IVideoDriver* getVideoDriver() { return nullptr; }
            virtual gui::IGUIEnvironment* getGUIEnvironment() { return nullptr; }
            virtual IMeshSceneNode* addCubeSceneNode(f32 size = 10.0f, ISceneNode* parent = nullptr,
                s32 id = -1, const core::vector3df& position = core::vector3df(0, 0, 0),
                const core::vector3df& rotation = core::vector3df(0, 0, 0),
                const core::vector3df& scale = core::vector3df(1, 1, 1)) {
                (void)size; (void)parent; (void)id; (void)position; (void)rotation; (void)scale;
                return nullptr;
            }
            virtual ICameraSceneNode* getActiveCamera() { return nullptr; }
            virtual ISceneCollisionManager* getSceneCollisionManager() { return nullptr; }
        };
        
        class ICameraSceneNode : public ISceneNode {
        public:
            virtual ~ICameraSceneNode() = default;
            virtual core::vector3df getTarget() const { return core::vector3df(0, 0, 100); }
            virtual void setTarget(const core::vector3df& target) { (void)target; }
        };
        
        class IMeshSceneNode : public ISceneNode {
        public:
            virtual ~IMeshSceneNode() = default;
            virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool value) {
                (void)flag; (void)value;
            }
            virtual void setMaterialTexture(u32 textureLayer, video::ITexture* texture) {
                (void)textureLayer; (void)texture;
            }
            virtual void setName(const char* name) { (void)name; }
        };
        
        class ISceneCollisionManager {
        public:
            virtual ~ISceneCollisionManager() = default;
            virtual core::line3d<f32> getRayFromScreenCoordinates(
                const core::position2d<s32>& pos, ICameraSceneNode* camera = nullptr) {
                (void)pos; (void)camera;
                return core::line3d<f32>();
            }
            virtual ISceneNode* getSceneNodeFromRayBB(const core::line3d<f32>& ray,
                s32 idBitMask = 0, bool noDebugObjects = false, ISceneNode* root = nullptr) {
                (void)ray; (void)idBitMask; (void)noDebugObjects; (void)root;
                return nullptr;
            }
        };
    }
    
    // Event system mocks
    enum EEVENT_TYPE {
        EET_GUI_EVENT = 0,
        EET_MOUSE_INPUT_EVENT,
        EET_KEY_INPUT_EVENT,
        EET_JOYSTICK_INPUT_EVENT,
        EET_LOG_TEXT_EVENT,
        EET_USER_EVENT
    };
    
    enum EKEY_CODE {
        KEY_LBUTTON = 0x01,
        KEY_RBUTTON = 0x02,
        KEY_CANCEL = 0x03,
        KEY_MBUTTON = 0x04,
        KEY_XBUTTON1 = 0x05,
        KEY_XBUTTON2 = 0x06,
        KEY_BACK = 0x08,
        KEY_TAB = 0x09,
        KEY_CLEAR = 0x0C,
        KEY_RETURN = 0x0D,
        KEY_SHIFT = 0x10,
        KEY_CONTROL = 0x11,
        KEY_MENU = 0x12,
        KEY_PAUSE = 0x13,
        KEY_CAPITAL = 0x14,
        KEY_KANA = 0x15,
        KEY_HANGUEL = 0x15,
        KEY_HANGUL = 0x15,
        KEY_JUNJA = 0x17,
        KEY_FINAL = 0x18,
        KEY_HANJA = 0x19,
        KEY_KANJI = 0x19,
        KEY_ESCAPE = 0x1B,
        KEY_CONVERT = 0x1C,
        KEY_NONCONVERT = 0x1D,
        KEY_ACCEPT = 0x1E,
        KEY_MODECHANGE = 0x1F,
        KEY_SPACE = 0x20,
        KEY_PRIOR = 0x21,
        KEY_NEXT = 0x22,
        KEY_END = 0x23,
        KEY_HOME = 0x24,
        KEY_LEFT = 0x25,
        KEY_UP = 0x26,
        KEY_RIGHT = 0x27,
        KEY_DOWN = 0x28,
        KEY_SELECT = 0x29,
        KEY_PRINT = 0x2A,
        KEY_EXECUT = 0x2B,
        KEY_SNAPSHOT = 0x2C,
        KEY_INSERT = 0x2D,
        KEY_DELETE = 0x2E,
        KEY_HELP = 0x2F,
        KEY_KEY_0 = 0x30,
        KEY_KEY_1 = 0x31,
        KEY_KEY_2 = 0x32,
        KEY_KEY_3 = 0x33,
        KEY_KEY_4 = 0x34,
        KEY_KEY_5 = 0x35,
        KEY_KEY_6 = 0x36,
        KEY_KEY_7 = 0x37,
        KEY_KEY_8 = 0x38,
        KEY_KEY_9 = 0x39,
        KEY_KEY_A = 0x41,
        KEY_KEY_B = 0x42,
        KEY_KEY_C = 0x43,
        KEY_KEY_D = 0x44,
        KEY_KEY_E = 0x45,
        KEY_KEY_F = 0x46,
        KEY_KEY_G = 0x47,
        KEY_KEY_H = 0x48,
        KEY_KEY_I = 0x49,
        KEY_KEY_J = 0x4A,
        KEY_KEY_K = 0x4B,
        KEY_KEY_L = 0x4C,
        KEY_KEY_M = 0x4D,
        KEY_KEY_N = 0x4E,
        KEY_KEY_O = 0x4F,
        KEY_KEY_P = 0x50,
        KEY_KEY_Q = 0x51,
        KEY_KEY_R = 0x52,
        KEY_KEY_S = 0x53,
        KEY_KEY_T = 0x54,
        KEY_KEY_U = 0x55,
        KEY_KEY_V = 0x56,
        KEY_KEY_W = 0x57,
        KEY_KEY_X = 0x58,
        KEY_KEY_Y = 0x59,
        KEY_KEY_Z = 0x5A,
        KEY_LWIN = 0x5B,
        KEY_RWIN = 0x5C,
        KEY_APPS = 0x5D,
        KEY_SLEEP = 0x5F,
        KEY_NUMPAD0 = 0x60,
        KEY_NUMPAD1 = 0x61,
        KEY_NUMPAD2 = 0x62,
        KEY_NUMPAD3 = 0x63,
        KEY_NUMPAD4 = 0x64,
        KEY_NUMPAD5 = 0x65,
        KEY_NUMPAD6 = 0x66,
        KEY_NUMPAD7 = 0x67,
        KEY_NUMPAD8 = 0x68,
        KEY_NUMPAD9 = 0x69,
        KEY_MULTIPLY = 0x6A,
        KEY_ADD = 0x6B,
        KEY_SEPARATOR = 0x6C,
        KEY_SUBTRACT = 0x6D,
        KEY_DECIMAL = 0x6E,
        KEY_DIVIDE = 0x6F,
        KEY_F1 = 0x70,
        KEY_F2 = 0x71,
        KEY_F3 = 0x72,
        KEY_F4 = 0x73,
        KEY_F5 = 0x74,
        KEY_F6 = 0x75,
        KEY_F7 = 0x76,
        KEY_F8 = 0x77,
        KEY_F9 = 0x78,
        KEY_F10 = 0x79,
        KEY_F11 = 0x7A,
        KEY_F12 = 0x7B,
        KEY_F13 = 0x7C,
        KEY_F14 = 0x7D,
        KEY_F15 = 0x7E,
        KEY_F16 = 0x7F,
        KEY_F17 = 0x80,
        KEY_F18 = 0x81,
        KEY_F19 = 0x82,
        KEY_F20 = 0x83,
        KEY_F21 = 0x84,
        KEY_F22 = 0x85,
        KEY_F23 = 0x86,
        KEY_F24 = 0x87,
        KEY_NUMLOCK = 0x90,
        KEY_SCROLL = 0x91,
        KEY_LSHIFT = 0xA0,
        KEY_RSHIFT = 0xA1,
        KEY_LCONTROL = 0xA2,
        KEY_RCONTROL = 0xA3,
        KEY_LMENU = 0xA4,
        KEY_RMENU = 0xA5,
        KEY_OEM_1 = 0xBA,
        KEY_PLUS = 0xBB,
        KEY_COMMA = 0xBC,
        KEY_MINUS = 0xBD,
        KEY_PERIOD = 0xBE,
        KEY_OEM_2 = 0xBF,
        KEY_OEM_3 = 0xC0,
        KEY_OEM_4 = 0xDB,
        KEY_OEM_5 = 0xDC,
        KEY_OEM_6 = 0xDD,
        KEY_OEM_7 = 0xDE,
        KEY_OEM_8 = 0xDF,
        KEY_OEM_AX = 0xE1,
        KEY_OEM_102 = 0xE2,
        KEY_ATTN = 0xF6,
        KEY_CRSEL = 0xF7,
        KEY_EXSEL = 0xF8,
        KEY_EREOF = 0xF9,
        KEY_PLAY = 0xFA,
        KEY_ZOOM = 0xFB,
        KEY_PA1 = 0xFD,
        KEY_OEM_CLEAR = 0xFE,
        KEY_NONE = 0xFF,
        KEY_KEY_CODES_COUNT = 0x100
    };
    
    enum EMOUSE_INPUT_EVENT {
        EMIE_LMOUSE_PRESSED_DOWN = 0,
        EMIE_RMOUSE_PRESSED_DOWN,
        EMIE_MMOUSE_PRESSED_DOWN,
        EMIE_LMOUSE_LEFT_UP,
        EMIE_RMOUSE_LEFT_UP,
        EMIE_MMOUSE_LEFT_UP,
        EMIE_MOUSE_MOVED,
        EMIE_MOUSE_WHEEL,
        EMIE_LMOUSE_DOUBLE_CLICK,
        EMIE_RMOUSE_DOUBLE_CLICK,
        EMIE_MMOUSE_DOUBLE_CLICK,
        EMIE_LMOUSE_TRIPLE_CLICK,
        EMIE_RMOUSE_TRIPLE_CLICK,
        EMIE_MMOUSE_TRIPLE_CLICK,
        EMIE_COUNT
    };
    
    namespace gui {
        enum EGUI_EVENT_TYPE {
            EGET_BUTTON_CLICKED = 0,
            EGET_ELEMENT_FOCUS_LOST,
            EGET_ELEMENT_FOCUSED,
            EGET_ELEMENT_HOVERED,
            EGET_ELEMENT_LEFT,
            EGET_ELEMENT_CLOSED,
            EGET_COUNT
        };
    }
    
    struct SEvent {
        EEVENT_TYPE EventType;
        
        struct SKeyInput {
            EKEY_CODE Key;
            bool PressedDown;
            bool Shift;
            bool Control;
            wchar_t Char;
        } KeyInput;
        
        struct SMouseInput {
            s32 X;
            s32 Y;
            f32 Wheel;
            bool Shift;
            bool Control;
            EMOUSE_INPUT_EVENT Event;
        } MouseInput;
        
        struct SGUIEvent {
            gui::EGUI_EVENT_TYPE EventType;
            gui::IGUIElement* Caller;
            gui::IGUIElement* Element;
        } GUIEvent;
    };
    
    namespace gui {
        
        class IGUIElement {
        public:
            virtual ~IGUIElement() = default;
            virtual s32 getID() const { return 0; }
        };
        
        class IGUIStaticText {
        public:
            virtual ~IGUIStaticText() = default;
            virtual void setText(const wchar_t* text) { (void)text; }
            virtual void setDrawBorder(bool border) { (void)border; }
            virtual void setOverrideColor(video::SColor color) { (void)color; }
            virtual void setBackgroundColor(video::SColor color) { (void)color; }
            virtual void setOverrideFont(IGUIFont* font) { (void)font; }
            virtual const wchar_t* getText() const { return L""; }
        };
        
        class IGUIFont {
        public:
            virtual ~IGUIFont() = default;
        };
        
        class IGUIButton : public IGUIElement {
        public:
            virtual ~IGUIButton() = default;
            virtual void setOverrideFont(IGUIFont* font) { (void)font; }
            virtual void setUseAlphaChannel(bool useAlpha) { (void)useAlpha; }
            virtual void setDrawBorder(bool border) { (void)border; }
            virtual void setIsPushButton(bool isPushButton) { (void)isPushButton; }
            virtual void setPressed(bool pressed) { (void)pressed; }
            virtual void setID(s32 id) { (void)id; }
        };
        
        class IGUIEnvironment {
        public:
            virtual ~IGUIEnvironment() = default;
            virtual void drawAll() {}
            virtual IGUIStaticText* addStaticText(const wchar_t* text,
                const core::rect<s32>& rectangle, bool border = false,
                bool wordWrap = true, IGUIElement* parent = nullptr,
                s32 id = -1, bool fillBackground = false) {
                (void)text; (void)rectangle; (void)border; (void)wordWrap;
                (void)parent; (void)id; (void)fillBackground;
                return nullptr;
            }
            virtual IGUIFont* getFont(const io::path& filename) {
                (void)filename;
                return nullptr;
            }
            virtual IGUIButton* addButton(const core::rect<s32>& rectangle,
                IGUIElement* parent = nullptr, s32 id = -1,
                const wchar_t* text = nullptr, const wchar_t* tooltiptext = nullptr) {
                (void)rectangle; (void)parent; (void)id; (void)text; (void)tooltiptext;
                return nullptr;
            }
        };
    }
    
    class ITimer {
    public:
        virtual ~ITimer() = default;
        virtual u32 getTime() const { return 0; }
    };
    
    class IEventReceiver {
    public:
        virtual ~IEventReceiver() = default;
        virtual bool OnEvent(const SEvent& event) = 0;
    };
    
    class IrrlichtDevice {
    public:
        virtual ~IrrlichtDevice() = default;
        virtual scene::ISceneManager* getSceneManager() = 0;
        virtual video::IVideoDriver* getVideoDriver() = 0;
        virtual io::IFileSystem* getFileSystem() = 0;
        virtual gui::IGUIEnvironment* getGUIEnvironment() = 0;
        virtual bool run() = 0;
        virtual void drop() = 0;
        virtual void setWindowCaption(const wchar_t* text) { (void)text; }
        virtual ITimer* getTimer() = 0;
        virtual void closeDevice() {}
    };
    
    // Mock global function inside irr namespace
    inline IrrlichtDevice* createDevice(
        video::E_DRIVER_TYPE deviceType = video::EDT_SOFTWARE,
        const core::dimension2d<u32>& windowSize = core::dimension2d<u32>(640, 480),
        u32 bits = 16,
        bool fullscreen = false,
        bool stencilbuffer = false,
        bool vsync = false,
        IEventReceiver* receiver = nullptr) {
        (void)deviceType; (void)windowSize; (void)bits; (void)fullscreen; 
        (void)stencilbuffer; (void)vsync; (void)receiver;
        return nullptr;
    }
}

