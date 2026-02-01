#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SPOTS 10
#define UDP_SERVER_PORT 5678

PROCESS(sink_server_process, "Smart Parking Sink");
AUTOSTART_PROCESSES(&sink_server_process);

static struct uip_udp_conn *server_conn;

struct spot_info {
  int is_disabled; // 0 = normal, 1 = disabled
  int status;      // 0 = vacant, 1 = occupied
};

static struct spot_info spots[MAX_SPOTS];

PROCESS_THREAD(sink_server_process, ev, data)
{
  PROCESS_BEGIN();

  NETSTACK_ROUTING.root_start();

  server_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);
  udp_bind(server_conn, UIP_HTONS(UDP_SERVER_PORT));

  printf("Sink: Smart Parking Server Started\n");

  while(1) {
    PROCESS_YIELD();

    if(ev == tcpip_event && uip_newdata()) {
      char *recv_data = (char *)uip_appdata;
      recv_data[uip_datalen()] = '\0';

      int id = UIP_IP_BUF->srcipaddr.u8[15]; // Last byte = mote ID
      int type = 0, spot = 0;
char *token;
token = strtok(recv_data, ",");
while(token != NULL) {
  if(strncmp(token, "type:", 5) == 0) {
    type = atoi(token + 5);
  } else if(strncmp(token, "spot:", 5) == 0) {
    spot = atoi(token + 5);
  }
  token = strtok(NULL, ",");
}

      spots[id].is_disabled = type;
      spots[id].status = spot;

      printf("Sink: Received from Mote %d -> Disabled: %d | Occupied: %d\n",
             id, spots[id].is_disabled, spots[id].status);

      // Assign spot for incoming vehicle
      int need_disabled_spot = 1; // Simulate if the car is a disabled driver
      printf("Sink: Checking for suitable spot...\n");

      for(int i = 2; i < MAX_SPOTS; i++) {
        if(spots[i].status == 0 &&
           ((need_disabled_spot && spots[i].is_disabled) ||
           (!need_disabled_spot && !spots[i].is_disabled))) {
          printf("Sink: Assigned Spot Mote %d (Disabled: %d)\n", i, spots[i].is_disabled);
          break;
        }
      }
    }
  }

  PROCESS_END();
}
