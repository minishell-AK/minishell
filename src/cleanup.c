#include "minishell.h"
#include <stdlib.h>
#include <readline/history.h>

static char ***g_envp_ref = NULL;
static int g_atexit_registered = 0;

static void
g_registered_cleanup(void)
{
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
    if (g_envp_ref && *g_envp_ref)
    {
        free_envp(*g_envp_ref);
        *g_envp_ref = NULL;
    }
    clear_history();
    g_envp_ref = NULL;
    exit(status);
}
