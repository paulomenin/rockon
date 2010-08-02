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

#include <Ecore.h>
#include <Eina.h>
#include <Elementary.h>
#include "rockon_data.h"
#include "xmms_conn.h"
#include "commands.h"
#include "gui.h"

/* eina_log domains */
int config_log_dom = -1;
int conn_log_dom   = -1;
int cmd_log_dom    = -1;
int ui_upd_log_dom = -1;
int playlist_log_dom = -1;
int broadcast_log_dom = -1;
int collection_log_dom = -1;

Eina_Bool log_init(void);
void log_shutdown(void);
void clean_resources(rockon_data* rdata);

EAPI int elm_main (int argc, char** argv) {
	rockon_data *rdata;

	if (log_init() == EINA_FALSE) {
		EINA_LOG_ERR("Log domains init failed");
	}

	rdata = rockon_data_new();

	if (rdata->config->edj_data_path == NULL) {
		printf("Critical Error: theme: %s not found.\n", rdata->config->theme);
		clean_resources(rdata);
		return 1;
	}

	if (!gui_window_set(rdata)) {
		printf("Critical Error: %s is a bad theme.\n", rdata->config->theme);
		clean_resources(rdata);
		return 2;
	}

	xmms2_connect(rdata);
	if (rdata->connection == NULL) {
		cmd_server_launch(rdata);
	}

	EINA_LOG_DBG("MainLoop Start");
	elm_run();
	EINA_LOG_DBG("MainLoop End");

	clean_resources(rdata);

	return 0;
}
ELM_MAIN()

void clean_resources(rockon_data* rdata) {
	rockon_data_del(rdata);
	log_shutdown();
	elm_shutdown();
}

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
	if (ui_upd_log_dom < 0) {
		ui_upd_log_dom = eina_log_domain_register("rck_UIupd", NULL);
	} else return EINA_FALSE;
	if (playlist_log_dom < 0) {
		playlist_log_dom = eina_log_domain_register("rck_pls", NULL);
	} else return EINA_FALSE;
	if (broadcast_log_dom < 0) {
		broadcast_log_dom = eina_log_domain_register("rck_bcast", NULL);
	} else return EINA_FALSE;
	if (collection_log_dom < 0) {
		collection_log_dom = eina_log_domain_register("rck_coll", NULL);
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
	if (ui_upd_log_dom >= 0) {
		eina_log_domain_unregister(ui_upd_log_dom);
		ui_upd_log_dom = -1;
	}
	if (playlist_log_dom >= 0) {
		eina_log_domain_unregister(playlist_log_dom);
		playlist_log_dom = -1;
	}
	if (broadcast_log_dom >= 0) {
		eina_log_domain_unregister(broadcast_log_dom);
		broadcast_log_dom = -1;
	}
	if (collection_log_dom >= 0) {
		eina_log_domain_unregister(collection_log_dom);
		collection_log_dom = -1;
	}
}
