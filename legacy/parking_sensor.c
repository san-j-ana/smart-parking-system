#include "contiki.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/linkaddr.h"  // For node_id
#include <stdio.h>
#include <string.h>

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678
#define SEND_INTERVAL (30 * CLOCK_SECOND)

PROCESS(parking_sensor_process, "Parking Sensor Node");
AUTOSTART_PROCESSES(&parking_sensor_process);

static struct uip_udp_conn *client_conn;
static struct etimer periodic_timer;

static int car_present = 0;
static int is_disabled_spot = 0; // 0 = normal, 1 = disabled

PROCESS_THREAD(parking_sensor_process, ev, data)
{
  uip_ipaddr_t dest_ipaddr;
  static char msg[40];

  PROCESS_BEGIN();

  // Assign type based on mote ID
  int node_id = linkaddr_node_addr.u8[7];  // Typically last byte of address
if(node_id == 2 || node_id == 4)
 {
    is_disabled_spot = 1;
  } else {
    is_disabled_spot = 0;
  }

  printf("Mote ID: %d, Type: %d\n", node_id, is_disabled_spot);

  client_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);
  udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT));

  etimer_set(&periodic_timer, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    car_present = !car_present; // simulate car arrival/departure

    if(NETSTACK_ROUTING.node_is_reachable() &&
       NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {

      snprintf(msg, sizeof(msg), "type:%d,spot:%d", is_disabled_spot, car_present);
      uip_udp_packet_sendto(client_conn, msg, strlen(msg),
                            &dest_ipaddr, UIP_HTONS(UDP_SERVER_PORT));

      printf("Sensor: Sent status %s\n", msg);
    } else {
      printf("Sensor: RPL root not reachable yet\n");
    }

    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
