#pragma once

#include <Core/String.h>

class EditorPanelBase {
public:
    EditorPanelBase(const String& name);

    bool open = true;

private:
    String m_cmdName;
};