#ifndef __IW_H
#define __IW_H

#include <stdbool.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>

#include "nl80211.h"

#define ETH_ALEN 6

/* libnl 1.x compatibility code */
#if !defined(CONFIG_LIBNL20) && !defined(CONFIG_LIBNL30)
#  define nl_sock nl_handle
#endif

struct nl80211_state {
	struct nl_sock *nl_sock;
	int nl80211_id;
};

enum command_identify_by {
	CIB_NONE,
	CIB_PHY,
	CIB_NETDEV,
};

enum id_input {
	II_NONE,
	II_NETDEV,
	II_PHY_NAME,
	II_PHY_IDX,
};

struct cmd {
	const char *name;
	const char *args;
	const char *help;
	const enum nl80211_commands cmd;
	int nl_msg_flags;
	int hidden;
	const enum command_identify_by idby;
	/*
	 * The handler should return a negative error code,
	 * zero on success, 1 if the arguments were wrong
	 * and the usage message should and 2 otherwise.
	 */
	int (*handler)(struct nl80211_state *state,
		       struct nl_cb *cb,
		       struct nl_msg *msg,
		       int argc, char **argv);
	const struct cmd *(*selector)(int argc, char **argv);
	const struct cmd *parent;
};

#define ARRAY_SIZE(ar) (sizeof(ar)/sizeof(ar[0]))
#define DIV_ROUND_UP(x, y) (((x) + (y - 1)) / (y))

#define __COMMAND(_section, _symname, _name, _args, _nlcmd, _flags, _hidden, _idby, _handler, _help, _sel)\
	static struct cmd						\
	__cmd ## _ ## _symname ## _ ## _handler ## _ ## _nlcmd ## _ ## _idby ## _ ## _hidden\
	__attribute__((used)) __attribute__((section("__cmd")))	= {	\
		.name = (_name),					\
		.args = (_args),					\
		.cmd = (_nlcmd),					\
		.nl_msg_flags = (_flags),				\
		.hidden = (_hidden),					\
		.idby = (_idby),					\
		.handler = (_handler),					\
		.help = (_help),					\
		.parent = _section,					\
		.selector = (_sel),					\
	}
#define __ACMD(_section, _symname, _name, _args, _nlcmd, _flags, _hidden, _idby, _handler, _help, _sel, _alias)\
	__COMMAND(_section, _symname, _name, _args, _nlcmd, _flags, _hidden, _idby, _handler, _help, _sel);\
	static const struct cmd *_alias = &__cmd ## _ ## _symname ## _ ## _handler ## _ ## _nlcmd ## _ ## _idby ## _ ## _hidden
#define COMMAND(section, name, args, cmd, flags, idby, handler, help)	\
	__COMMAND(&(__section ## _ ## section), name, #name, args, cmd, flags, 0, idby, handler, help, NULL)
#define COMMAND_ALIAS(section, name, args, cmd, flags, idby, handler, help, selector, alias)\
	__ACMD(&(__section ## _ ## section), name, #name, args, cmd, flags, 0, idby, handler, help, selector, alias)
#define HIDDEN(section, name, args, cmd, flags, idby, handler)		\
	__COMMAND(&(__section ## _ ## section), name, #name, args, cmd, flags, 1, idby, handler, NULL, NULL)

#define TOPLEVEL(_name, _args, _nlcmd, _flags, _idby, _handler, _help)	\
	struct cmd							\
	__section ## _ ## _name						\
	__attribute__((used)) __attribute__((section("__cmd")))	= {	\
		.name = (#_name),					\
		.args = (_args),					\
		.cmd = (_nlcmd),					\
		.nl_msg_flags = (_flags),				\
		.idby = (_idby),					\
		.handler = (_handler),					\
		.help = (_help),					\
	 }
#define SECTION(_name)							\
	struct cmd __section ## _ ## _name				\
	__attribute__((used)) __attribute__((section("__cmd"))) = {	\
		.name = (#_name),					\
		.hidden = 1,						\
	}

#define DECLARE_SECTION(_name)						\
	extern struct cmd __section ## _ ## _name;

extern const char iw_version[];

extern int iw_debug;

int handle_cmd(struct nl80211_state *state, enum id_input idby,
	       int argc, char **argv);

struct print_event_args {
	struct timeval ts; /* internal */
	bool have_ts; /* must be set false */
	bool frame, time, reltime;
};

__u32 listen_events(struct nl80211_state *state,
		    const int n_waits, const __u32 *waits);
int __prepare_listen_events(struct nl80211_state *state);
__u32 __do_listen_events(struct nl80211_state *state,
			 const int n_waits, const __u32 *waits,
			 struct print_event_args *args);

/* 802.11n HT capability AMPDU settings (for ampdu_params_info) */
#define IEEE80211_HT_AMPDU_PARM_FACTOR          0x03
#define IEEE80211_HT_AMPDU_PARM_DENSITY         0x1C

#define IEEE80211_HT_CAP_SUP_WIDTH_20_40        0x0002
#define IEEE80211_HT_CAP_SGI_40                 0x0040
#define IEEE80211_HT_CAP_MAX_AMSDU              0x0800

#define IEEE80211_HT_MCS_MASK_LEN               10

/**
 * struct ieee80211_mcs_info - MCS information
 * @rx_mask: RX mask
 * @rx_highest: highest supported RX rate. If set represents
 *      the highest supported RX data rate in units of 1 Mbps.
 *      If this field is 0 this value should not be used to
 *      consider the highest RX data rate supported.
 * @tx_params: TX parameters
 */
struct ieee80211_mcs_info {
	__u8 rx_mask[IEEE80211_HT_MCS_MASK_LEN];
	__u16 rx_highest;
	__u8 tx_params;
	__u8 reserved[3];
} __attribute__ ((packed));


/**
 * struct ieee80211_ht_cap - HT capabilities
 *
 * This structure is the "HT capabilities element" as
 * described in 802.11n D5.0 7.3.2.57
 */
struct ieee80211_ht_cap {
	__u16 cap_info;
	__u8 ampdu_params_info;

	/* 16 bytes MCS information */
	struct ieee80211_mcs_info mcs;

	__u16 extended_ht_cap_info;
	__u32 tx_BF_cap_info;
	__u8 antenna_selection_info;
} __attribute__ ((packed));

unsigned short cpu_to_le16(unsigned short v);
int mac_addr_a2n(unsigned char *mac_addr, char *arg);
void mac_addr_n2a(char *mac_addr, unsigned char *arg);
int parse_hex_mask(char *hexmask, unsigned char **result, size_t *result_len,
		   unsigned char **mask);
unsigned char *parse_hex(char *hex, size_t *outlen);

int parse_keys(struct nl_msg *msg, char **argv, int argc);

void print_ht_mcs(const __u8 *mcs);
void print_ampdu_length(__u8 exponent);
void print_ampdu_spacing(__u8 spacing);
void print_ht_capability(__u16 cap);

const char *iftype_name(enum nl80211_iftype iftype);
const char *command_name(enum nl80211_commands cmd);
int ieee80211_channel_to_frequency(int chan);
int ieee80211_frequency_to_channel(int freq);

void print_ssid_escaped(const uint8_t len, const uint8_t *data);

int nl_get_multicast_id(struct nl_sock *sock, const char *family, const char *group);

char *reg_initiator_to_string(__u8 initiator);

const char *get_reason_str(uint16_t reason);
const char *get_status_str(uint16_t status);

enum print_ie_type {
	PRINT_SCAN,
	PRINT_LINK,
};

#define BIT(x) (1ULL<<(x))

void print_ies(unsigned char *ie, int ielen, bool unknown,
	       enum print_ie_type ptype);


DECLARE_SECTION(set);
DECLARE_SECTION(get);

#endif /* __IW_H */
