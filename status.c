#include <stdint.h>
#include "iw.h"

/* This seems to be a decent reference:
 * https://www.wifi-professionals.com/2024/08/ieee-802-11-reason-codes-to-identify-network-issues
 */

static const char *status_table[] = {
	[0] = "Successful",
	[1] = "Unspecified failure",
	[2] = "Tunneled direct link setup (TDLS) wakeup schedule rejected but alternative schedule provided",
	[3] = "TDLS wakeup schedule rejected",
	[5] = "Security disabled",
	[6] = "Unacceptable lifetime",
	[7] = "Not in same basic service set (BSS)",
	[10] = "Cannot support all requested capabilities in the capability information field",
	[11] = "Reassociation denied due to inability to confirm that association exists",
	[12] = "Association denied due to reason outside the scope of this standard",
	[13] = "Responding station does not support the specified authentication algorithm",
	[14] = "Received an authentication frame with authentication transaction sequence number out of expected sequence",
	[15] = "Authentication rejected because of challenge failure",
	[16] = "Authentication rejected due to timeout waiting for next frame in sequence",
	[17] = "Association denied because AP is unable to handle additional associated STA",
	[18] = "Association denied due to requesting station not supporting all of the data rates in the BSSBasicRateSet parameter",
	[19] = "Association denied due to requesting station not supporting the short preamble option",
	[20] = "Association denied due to requesting station not supporting the PBCC modulation option",
	[21] = "Association denied due to requesting station not supporting the channel agility option",
	[22] = "Association request rejected because Spectrum Management capability is required",
	[23] = "Association request rejected because the information in the Power Capability element is unacceptable",
	[24] = "Association request rejected because the information in the Supported Channels element is unacceptable",
	[25] = "Association request rejected due to requesting station not supporting the short slot time option",
	[26] = "Association request rejected due to requesting station not supporting the ER-PBCC modulation option",
	[27] = "Association denied due to requesting STA not supporting HT features",
	[28] = "R0KH Unreachable",
	[29] = "Association denied because the requesting STA does not support the PCO transition required by the AP",
	[30] = "Association request rejected temporarily; try again later",
	[31] = "Robust Management frame policy violation",
	[32] = "Unspecified, QoS related failure",
	[33] = "Association denied due to QAP having insufficient bandwidth to handle another QSTA",
	[34] = "Association denied due to poor channel conditions",
	[35] = "Association (with QBSS) denied due to requesting station not supporting the QoS facility",
	[37] = "The request has been declined",
	[38] = "The request has not been successful as one or more parameters have invalid values",
	[39] = "The TS has not been created because the request cannot be honored. However, a suggested Tspec is provided so that the initiating QSTA may attempt to send another TS with the suggested changes to the TSpec",
	[40] = "Invalid Information Element",
	[41] = "Group Cipher is not valid",
	[42] = "Pairwise Cipher is not valid",
	[43] = "AKMP is not valid",
	[44] = "Unsupported RSN IE version",
	[45] = "Invalid RSN IE Capabilities",
	[46] = "Cipher suite is rejected per security policy",
	[47] = "The TS has not been created. However, the HC may be capable of creating a TS, in response to a request, after the time indicated in the TS Delay element",
	[48] = "Direct link is not allowed in the BSS by policy",
	[49] = "Destination STA is not present within this QBSS",
	[50] = "The destination STA is not a QSTA",
	[51] = "Association denied because Listen Interval is too large",
	[52] = "Invalid Fast BSS Transition Action Frame Count",
	[53] = "Invalid PMKID",
	[54] = "Invalid MDIE",
	[55] = "Invalid FTIE",
	[56] = "Requested traffic classification (TCLAS) processing not supported by AP",
	[57] = "AP has insufficient TCLAS processing resources to satisfy request",
	[58] = "TS not created because request can’t be honored; however, HC suggests station transitions to other BSSs to set up TS",
	[59] = "Generic advertisement service (GAS) advertisement protocol not supported",
	[60] = "No outstanding GAS request",
	[61] = "GAS response not received from advertisement server",
	[62] = "Station timed out waiting for GAS query response",
	[63] = "GAS response larger than query response length limit",
	[64] = "Request refused because home network doesn’t support request",
	[65] = "Advertisement server in network not currently reachable",
	[68] = "Request refused because AP doesn’t support unauthenticated access",
	[73] = "Unscheduled Automatic Power Save Delivery (U-APSD) coexistence not supported",
	[74] = "Requested U-APSD coexistence mode not supported",
	[75] = "Requested interval or duration value can’t be supported with U-APSD coexistence",
	[76] = "Authentication rejected because anti-clogging token is required",
	[77] = "Authentication rejected because the offered finite cyclic group not supported",
	[78] = "The Target Beacon Transmission Time (TBTT) adjustment request not successful because station couldn’t find alternative TBTT",
	[79] = "Transmission failure",
	[80] = "Requested TCLAS not supported",
	[81] = "TCLAS resources exhausted",
	[82] = "Rejected with suggested BSS transition",
	[93] = "Association or reassociation refused because of memory limits at AP",
	[94] = "Association or reassociation refused because emergency services not supported at AP",
	[95] = "GAS query response not yet received",
	[101] = "Request failed due to exceeded MCCA access fraction (MAF) limit",
	[102] = "Request failed due to exceeded MCF coordinated channel access (MCCA) track limit, where MCF refers to mesh coordination function",
};

const char *get_status_str(uint16_t status)
{
	if (status < ARRAY_SIZE(status_table) && status_table[status])
		return status_table[status];
	return "<unknown>";
}
