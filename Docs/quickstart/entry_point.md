# Entix's application entry point

:::{important}
All code written on top of Entix Engine should be placed to `namespace Entix`
:::

At start of development you should provide entry point to engine by writing
following code:

```{code-block} cpp
:caption: Main.cpp
:emphasize-lines: 5-10
:lineno-start: 1

#include <Entix/Game/EntryPoint.h>

namespace Entix
{
    ApplicationDesc CreateApplication()
    {
        return ApplicationDescBuilder()
            .Build()
            .Unwrap();
    }
}
```

The {cpp:func}`Entix::CreateApplication` are called inside {cpp:func}`Entix::Main` to create new application from your description.

* * *

::::{note}

The {cpp:func}`Entix::Main` are called inside platform-specific entry-point.

**Windows variant:**

:::{literalinclude} ../../Entix/Source/Entix/Game/EntryPoint.h
:language: cpp
:start-after: //! [entry_windows]
:end-before: //! [entry_windows]

:::

**Linux variant:**

:::{literalinclude} ../../Entix/Source/Entix/Game/EntryPoint.h
:language: cpp
:start-after: //! [entry_linux]
:end-before: //! [entry_linux]

:::

::::

* * *
