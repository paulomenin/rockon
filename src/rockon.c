/* This file is part of Rockon.
 * 
 * Rockon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * Rockon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Rockon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Elementary.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include "server_data.h"
#include "xmms_conn.h"
#include "commands.h"
#include "gui_callbacks.h"

/* eina_log domains */
int config_log_dom = -1;
int conn_log_dom = -1;
int cmd_log_dom = -1;
int gui_upd_log_dom = -1;
int gui_window_log_dom = -1;
int playlist_log_dom = -1;

Eina_Bool log_init(void);
void log_shutdown(void);

EAPI int elm_main (int argc, char** argv) {
	server_data *sdata;

	if (log_init() == EINA_FALSE) {
		EINA_LOG_ERR("Log domains init failed");
	}

	sdata = server_data_new();

	xmms2_connect(sdata);

	if (sdata->connection == NULL) {
		cmd_server_launch(sdata);
	}

	elm_cb_set (sdata, NULL, "Rockon", "elm_set,win");

	EINA_LOG_DBG("MainLoop Start");
	elm_run();
	EINA_LOG_DBG("MainLoop End");

	if (sdata->ecore_fdh != NULL) {
		xmmsc_mainloop_ecore_shutdown(sdata->connection, sdata->ecore_fdh);
		sdata->ecore_fdh = NULL;
	}

	if (sdata->config->terminate_server == 1) {
		cmd_server_shutdown(sdata);
	}

	server_data_del(sdata);
	log_shutdown();
	elm_shutdown();

	return 0;
}
ELM_MAIN()

Eina_Bool log_init(void) {
	if (config_log_dom < 0) {
		config_log_dom = eina_log_domain_register("rck_config", NULL);
	} else return EINA_FALSE;
	if (conn_log_dom < 0) {
		conn_log_dom = eina_log_domain_register("rck_conn", NULL);
	} else return EINA_FALSE;
	if (cmd_log_dom < 0) {
		cmd_log_dom = eina_log_domain_register("rck_cmd", NULL);
	} else return EINA_FALSE;
	if (gui_upd_log_dom < 0) {
		gui_upd_log_dom = eina_log_domain_register("rck_GUIupd", NULL);
	} else return EINA_FALSE;
	if (playlist_log_dom < 0) {
		playlist_log_dom = eina_log_domain_register("rck_pls", NULL);
	} else return EINA_FALSE;
	if (gui_window_log_dom < 0) {
		gui_window_log_dom = eina_log_domain_register("rck_window", NULL);
	} else return EINA_FALSE;

	return EINA_TRUE;
}

void log_shutdown(void) {
	if (config_log_dom >= 0) {
		eina_log_domain_unregister(config_log_dom);
		config_log_dom = -1;
	}
	if (conn_log_dom >= 0) {
		eina_log_domain_unregister(conn_log_dom);
		conn_log_dom = -1;
	}
	if (cmd_log_dom >= 0) {
		eina_log_domain_unregister(cmd_log_dom);
		cmd_log_dom = -1;
	}
	if (gui_upd_log_dom >= 0) {
		eina_log_domain_unregister(gui_upd_log_dom);
		gui_upd_log_dom = -1;
	}
	if (playlist_log_dom >= 0) {
		eina_log_domain_unregister(playlist_log_dom);
		playlist_log_dom = -1;
	}
	if (gui_window_log_dom >= 0) {
		eina_log_domain_unregister(gui_window_log_dom);
		gui_window_log_dom = -1;
	}
}
