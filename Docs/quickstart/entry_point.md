# Entix's application entry point

:::{important}
All code written on top of Entix Engine should be placed to `namespace Entix`
:::

## Game entry-point

At start of development you should provide entry point to engine by writing
following code:

```{code-block} cpp
:caption: Main.cpp
:emphasize-lines: 1,5-10
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

:::{seealso}
{doc}`/quickstart/application_and_applicationdesc` page for more info about {cpp:class}`Entix::Application`, {cpp:class}`Entix::ApplicationDesc` and {cpp:class}`Entix::ApplicationDescBuilder`.
:::

* * *

Game entry-point are used for simplicity and "sugar". A lot of work are done for you with {cpp:class}`Entix::Application`. In {cpp:class}`Entix::ApplicationDesc` you can specify a lot of things.

::::{card} `#include <Entix/Game/Application.h>`
**ApplicationDesc builder class**
^^^
:::{doxygenclass} Entix::ApplicationDescBuilder
:project: Entix Engine
:members:
:undoc-members:

:::

::::

## Raw entry-point

If you don't need built-in {cpp:class}`Entix::Application` class you can write your own things in "raw" entry-point provided by Entix Runtime.

Difference are between included headers and functions.

Instead of `Entix/Game/EntryPoint.h` you need to include `Entix/Core/EntryPoint.h` and write following code:

```{code-block} cpp
:caption: Main.cpp
:emphasize-lines: 1,5-8
:lineno-start: 1

#include <Entix/Core/EntryPoint.h>

namespace Entix
{
    int Main(int argc, char** argv)
    {
        return 0;
    }
}
```

* * *

::::{note}

The {cpp:func}`Entix::Main` are called inside platform-specific entry-point.

**Windows variant:**

:::{literalinclude} ../../Entix/Source/Entix/Core/EntryPoint.h
:language: cpp
:start-after: //! [entry_windows]
:end-before: //! [entry_windows]

:::

**Linux variant:**

:::{literalinclude} ../../Entix/Source/Entix/Core/EntryPoint.h
:language: cpp
:start-after: //! [entry_linux]
:end-before: //! [entry_linux]

:::

::::
