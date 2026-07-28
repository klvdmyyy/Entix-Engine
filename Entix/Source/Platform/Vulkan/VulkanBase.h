#pragma once

#define EX_VK_TRY(EXPR) \
    do { try { EXPR } catch(const vk::SystemError& e) { return Error(e.what()); } catch (const std::exception& e) { return Error(e.what()); } } while(0)

namespace Entix
{
}