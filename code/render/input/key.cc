//------------------------------------------------------------------------------
//  key.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/key.h"

namespace Input
{
Util::Dictionary<Util::String,Key::Code> Key::dict;

//------------------------------------------------------------------------------
/**
*/
Util::String
Key::ToString(Code code)
{
    switch (code)
    {
        case Back:          return "Back";
        case Tab:           return "Tab";
        case Clear:         return "Clear";
        case Return:        return "Return";
        case Shift:         return "Shift";
        case Control:       return "Control";
        case Menu:          return "Menu";
        case Pause:         return "Pause";
        case Capital:       return "Capital";
        case Escape:        return "Escape";
        case Convert:       return "Convert";
        case NonConvert:    return "NonConvert";
        case Accept:        return "Accept";
        case ModeChange:    return "ModeChange";
        case Space:         return "Space";
        case Prior:         return "Prior";
        case Next:          return "Next";
        case End:           return "End";
        case Home:          return "Home";
        case Left:          return "Left";
        case Right:         return "Right";
        case Up:            return "Up";
        case Down:          return "Down";
        case Select:        return "Select";
        case Print:         return "Print";
        case Execute:       return "Execute";
        case Snapshot:      return "Snapshot";
        case Insert:        return "Insert";
        case Delete:        return "Delete";
        case Help:          return "Help";
        case LeftWindows:   return "LeftWindows";
        case RightWindows:  return "RightWindows";
        case Apps:          return "Apps";
        case Sleep:         return "Sleep";
        case NumPad0:       return "NumPad0";
        case NumPad1:       return "NumPad1";
        case NumPad2:       return "NumPad2";
        case NumPad3:       return "NumPad3";
        case NumPad4:       return "NumPad4";
        case NumPad5:       return "NumPad5";
        case NumPad6:       return "NumPad6";
        case NumPad7:       return "NumPad7";
        case NumPad8:       return "NumPad8";
        case NumPad9:       return "NumPad9";
        case Multiply:      return "Multiply";
        case Add:           return "Add";
        case Subtract:      return "Subtract";
        case Separator:     return "Separator";
        case Decimal:       return "Decimal";
        case Divide:        return "Divide";
        case F1:            return "F1";
        case F2:            return "F2";
        case F3:            return "F3";
        case F4:            return "F4";
        case F5:            return "F5";
        case F6:            return "F6";
        case F7:            return "F7";
        case F8:            return "F8";
        case F9:            return "F9";
        case F10:           return "F10";
        case F11:           return "F11";
        case F12:           return "F12";
        case F13:           return "F13";
        case F14:           return "F14";
        case F15:           return "F15";
        case F16:           return "F16";
        case F17:           return "F17";
        case F18:           return "F18";
        case F19:           return "F19";
        case F20:           return "F20";
        case F21:           return "F21";
        case F22:           return "F22";
        case F23:           return "F23";
        case F24:           return "F24";
        case NumLock:       return "NumLock";
        case Scroll:        return "Scroll";
        case Semicolon:     return "Semicolon";
        case Slash:         return "Slash";
        case Tilde:         return "Tilde";
        case LeftBracket:   return "LeftBracket";
        case RightBracket:  return "RightBracket";
        case BackSlash:     return "BackSlash";
        case Quote:         return "Quote";
        case Comma:         return "Comma";
        case Underbar:      return "Underbar";
        case Period:        return "Period";
        case Equality:      return "Equality";
        case LeftShift:     return "LeftShift";
        case RightShift:    return "RightShift";
        case LeftControl:   return "LeftControl";
        case RightControl:  return "RightControl";
        case LeftMenu:      return "LeftMenu";
        case RightMenu:     return "RightMenu";
        case BrowserBack:   return "BrowserBack";
        case BrowserForward:    return "BrowserForward";
        case BrowserRefresh:    return "BrowserRefresh";
        case BrowserStop:       return "BrowserStop";
        case BrowserSearch:     return "BrowserSearch";
        case BrowserFavorites:  return "BrowserFavorites";
        case BrowserHome:       return "BrowserHome";
        case VolumeMute:        return "VolumeMute";
        case VolumeDown:        return "VolumeDown";
        case VolumeUp:          return "VolumeUp";
        case MediaNextTrack:    return "MediaNextTrack";
        case MediaPrevTrack:    return "MediaPrevTrack";
        case MediaStop:         return "MediaStop";
        case MediaPlayPause:    return "MediaPlayPause";
        case LaunchMail:        return "LaunchMail";
        case LaunchMediaSelect: return "LaunchMediaSelect";
        case LaunchApp1:        return "LaunchApp1";
        case LaunchApp2:        return "LaunchApp2";
        case Key0:          return "Key0";
        case Key1:          return "Key1";
        case Key2:          return "Key2";
        case Key3:          return "Key3";
        case Key4:          return "Key4";
        case Key5:          return "Key5";
        case Key6:          return "Key6";
        case Key7:          return "Key7";
        case Key8:          return "Key8";
        case Key9:          return "Key9";
        case A:             return "A";
        case B:             return "B";
        case C:             return "C";
        case D:             return "D";
        case E:             return "E";
        case F:             return "F";
        case G:             return "G";
        case H:             return "H";
        case I:             return "I";
        case J:             return "J";
        case K:             return "K";
        case L:             return "L";
        case M:             return "M";
        case N:             return "N";
        case O:             return "O";
        case P:             return "P";
        case Q:             return "Q";
        case R:             return "R";
        case S:             return "S";
        case T:             return "T";
        case U:             return "U";
        case V:             return "V";
        case W:             return "W";
        case X:             return "X";
        case Y:             return "Y";
        case Z:             return "Z";    
		default:
			break;
    }
    n_error("Invalid key code!");
    return "";
}

//------------------------------------------------------------------------------
/**
*/
Key::Code
Key::FromString(const Util::String& str)
{
    // setup a static dictionary object when called first
    if (dict.IsEmpty())
    {
        dict.Add("Back", Back);
        dict.Add("Tab", Tab);
        dict.Add("Clear", Clear);
        dict.Add("Return", Return);
        dict.Add("Shift", Shift);
        dict.Add("Control", Control);
        dict.Add("Menu", Menu);
        dict.Add("Pause", Pause);
        dict.Add("Capital", Capital);
        dict.Add("Escape", Escape);
        dict.Add("Convert", Convert);
        dict.Add("NonConvert", NonConvert);
        dict.Add("Accept", Accept);
        dict.Add("ModeChange", ModeChange);
        dict.Add("Space", Space);
        dict.Add("Prior", Prior);
        dict.Add("Next", Next);
        dict.Add("End", End);
        dict.Add("Home", Home);
        dict.Add("Left", Left);
        dict.Add("Right", Right);
        dict.Add("Up", Up);
        dict.Add("Down", Down);
        dict.Add("Select", Select);
        dict.Add("Print", Print);
        dict.Add("Execute", Execute);
        dict.Add("Snapshot", Snapshot);
        dict.Add("Insert", Insert);
        dict.Add("Delete", Delete);
        dict.Add("Help", Help);
        dict.Add("LeftWindows", LeftWindows);
        dict.Add("RightWindows", RightWindows);
        dict.Add("Apps", Apps);
        dict.Add("Sleep", Sleep);
        dict.Add("NumPad0", NumPad0);
        dict.Add("NumPad1", NumPad1);
        dict.Add("NumPad2", NumPad2);
        dict.Add("NumPad3", NumPad3);
        dict.Add("NumPad4", NumPad4);
        dict.Add("NumPad5", NumPad5);
        dict.Add("NumPad6", NumPad6);
        dict.Add("NumPad7", NumPad7);
        dict.Add("NumPad8", NumPad8);
        dict.Add("NumPad9", NumPad9);
        dict.Add("Multiply", Multiply);
        dict.Add("Add", Add);
        dict.Add("Subtract", Subtract);
        dict.Add("Separator", Separator);
        dict.Add("Decimal", Decimal);
        dict.Add("Divide", Divide);
        dict.Add("F1", F1);
        dict.Add("F2", F2);
        dict.Add("F3", F3);
        dict.Add("F4", F4);
        dict.Add("F5", F5);
        dict.Add("F6", F6);
        dict.Add("F7", F7);
        dict.Add("F8", F8);
        dict.Add("F9", F9);
        dict.Add("F10", F10);
        dict.Add("F11", F11);
        dict.Add("F12", F12);
        dict.Add("F13", F13);
        dict.Add("F14", F14);
        dict.Add("F15", F15);
        dict.Add("F16", F16);
        dict.Add("F17", F17);
        dict.Add("F18", F18);
        dict.Add("F19", F19);
        dict.Add("F20", F20);
        dict.Add("F21", F21);
        dict.Add("F22", F22);
        dict.Add("F23", F23);
        dict.Add("F24", F24);
        dict.Add("NumLock", NumLock);
        dict.Add("Scroll", Scroll);
        dict.Add("Semicolon", Semicolon);
        dict.Add("Slash", Slash);
        dict.Add("Tilde", Tilde);
        dict.Add("LeftBracket", LeftBracket);
        dict.Add("RightBracket", RightBracket);
        dict.Add("BackSlash", BackSlash);
        dict.Add("Quote", Quote);
        dict.Add("Comma", Comma);
        dict.Add("Underbar", Underbar);
        dict.Add("Period", Period);
        dict.Add("Equality", Equality);
        dict.Add("LeftShift", LeftShift);
        dict.Add("RightShift", RightShift);
        dict.Add("LeftControl", LeftControl);
        dict.Add("RightControl", RightControl);
        dict.Add("LeftMenu", LeftMenu);
        dict.Add("RightMenu", RightMenu);
        dict.Add("BrowserBack", BrowserBack);
        dict.Add("BrowserForward", BrowserForward);
        dict.Add("BrowserRefresh", BrowserRefresh);
        dict.Add("BrowserStop", BrowserStop);
        dict.Add("BrowserSearch", BrowserSearch);
        dict.Add("BrowserFavorites", BrowserFavorites);
        dict.Add("BrowserHome", BrowserHome);
        dict.Add("VolumeMute", VolumeMute);
        dict.Add("VolumeDown", VolumeDown);
        dict.Add("VolumeUp", VolumeUp);
        dict.Add("MediaNextTrack", MediaNextTrack);
        dict.Add("MediaPrevTrack", MediaPrevTrack);
        dict.Add("MediaStop", MediaStop);
        dict.Add("MediaPlayPause", MediaPlayPause);
        dict.Add("LaunchMail", LaunchMail);
        dict.Add("LaunchMediaSelect", LaunchMediaSelect);
        dict.Add("LaunchApp1", LaunchApp1);
        dict.Add("LaunchApp2", LaunchApp2);
        dict.Add("Key0", Key0);
        dict.Add("Key1", Key1);
        dict.Add("Key2", Key2);
        dict.Add("Key3", Key3);
        dict.Add("Key4", Key4);
        dict.Add("Key5", Key5);
        dict.Add("Key6", Key6);
        dict.Add("Key7", Key7);
        dict.Add("Key8", Key8);
        dict.Add("Key9", Key9);
        dict.Add("A", A);
        dict.Add("B", B);
        dict.Add("C", C);
        dict.Add("D", D);
        dict.Add("E", E);
        dict.Add("F", F);
        dict.Add("G", G);
        dict.Add("H", H);
        dict.Add("I", I);
        dict.Add("J", J);
        dict.Add("K", K);
        dict.Add("L", L);
        dict.Add("M", M);
        dict.Add("N", N);
        dict.Add("O", O);
        dict.Add("P", P);
        dict.Add("Q", Q);
        dict.Add("R", R);
        dict.Add("S", S);
        dict.Add("T", T);
        dict.Add("U", U);
        dict.Add("V", V);
        dict.Add("W", W);
        dict.Add("X", X);
        dict.Add("Y", Y);
        dict.Add("Z", Z);
    }
    return dict[str];
}
    
//------------------------------------------------------------------------------
/**
*/
Util::Array<Key::Code>
Key::KeyCodesByGroup(Key::Group group)
{
    Util::Array<Key::Code> keys;
    IndexT index;
    switch (group)
    {
        case Key::Letters:        
            for (index = Key::A; index <= Key::Z; index++)
            {
                keys.Append((Key::Code)index);
            }
            break;
        case Key::Numbers:
            for (index = Key::Key0; index <= Key::Key9; index++)
            {
                keys.Append((Key::Code)index);
            }
            break;
        case Key::NumPad:      
            for (index = Key::NumPad0; index <= Key::Divide; index++)
            {
                keys.Append((Key::Code)index);
            }
            break;
        case Key::FunctionKeys:
            for (index = Key::F1; index <= Key::F24; index++)
            {
                keys.Append((Key::Code)index);
            }
            break;
        case Key::CursorKeys:
            for (index = Key::Left; index <= Key::Down; index++)
            {
                keys.Append((Key::Code)index);
            }
            break;
        case Key::HomeBlock:            
            keys.Append(Key::Prior);
            keys.Append(Key::Next);
            keys.Append(Key::Home);
            keys.Append(Key::End);
            keys.Append(Key::Insert);
            keys.Append(Key::Delete);
            break;

        default:
           n_error("Key::KeyCodesByGroup: Invalid Key Group.");
           break;
    }
    return keys;
}


} // namespace Input