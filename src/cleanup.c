#include "minishell.h"
#include <stdlib.h>
#include <readline/history.h>

/*
 * Small cleanup registry: allow registering the `char **` envp pointer
 * so that we can guarantee it gets freed on normal process exit.
 *
 * Rules:
 * - call `register_envp_ref(&my_env)` in `main` after `dup_envp`
 * - call `unregister_envp_ref()` if you manually free the env before exit
 */

static char ***g_envp_ref = NULL;
static int g_atexit_registered = 0;

static void
g_registered_cleanup(void)
{
    /* clear readline history (free history entries allocated by add_history) */
    clear_history();
    if (g_envp_ref && *g_envp_ref)
    {
        free_envp(*g_envp_ref);
        *g_envp_ref = NULL;
    }
}

void	register_envp_ref(char ***envp_ref)
{
    if (!envp_ref)
        return ;
    g_envp_ref = envp_ref;
    if (!g_atexit_registered)
    {
        atexit(g_registered_cleanup);
        g_atexit_registered = 1;
    }
}

void	unregister_envp_ref(void)
{
    g_envp_ref = NULL;
}

void	cleanup_and_exit(int status)
{
    /* Free registered envp immediately and avoid double-free in atexit */
    if (g_envp_ref && *g_envp_ref)
    {
        free_envp(*g_envp_ref);
        *g_envp_ref = NULL;
    }
    /* free readline history before exiting the parent process */
    clear_history();
    /* unregister so atexit handler does nothing */
    g_envp_ref = NULL;
    exit(status);
}
