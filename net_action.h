/*************************************************************************/
/*  net_actions.h**                                                      */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef NET_ACTIONS_H
#define NET_ACTIONS_H

#include "core/object/ref_counted.h"
#include "net_action_processor.h"
#include "net_utilities.h"

class SceneSynchronizer;
struct NetActionInfo;

struct SenderNetAction {
	/// The token used to reference this Action.
	uint32_t action_token = UINT32_MAX;
	/// The token generated by the peer that generated the Action: Usually this is the same as `action_token`.
	uint32_t triggerer_action_token = UINT32_MAX;
	NetActionProcessor action_processor;
	Vector<uint8_t> vars_buffer;
	Vector<int> recipients;
	int sender_peer = -1;
	HashMap<int, uint32_t> peers_executed_input_id;
	bool locally_executed = false;
	bool sent_by_the_server = false;
	uint32_t send_count = 0;
	uint32_t send_timestamp = 0;
	bool sender_executed_time_changed = false;

	void prepare_processor(NetUtility::NodeData *p_nd, NetActionId p_event_id, const Array &p_vars);
	const NetActionInfo &get_action_info() const;
	void client_set_executed_input_id(uint32_t p_input_id);
	uint32_t client_get_executed_input_id() const;

	uint32_t peer_get_executed_input_id(int p_peer) const;
};

namespace net_action {
void encode_net_action(
		const LocalVector<SenderNetAction *> &p_actions,
		int p_source_peer,
		DataBuffer &r_data_buffer);

void decode_net_action(
		SceneSynchronizer *synchronizer,
		DataBuffer &p_data_buffer,
		int p_source_peer,
		LocalVector<SenderNetAction> &r_actions);
}; //namespace net_action

struct NetActionSenderInfo {
	uint32_t last_received_action_id = UINT32_MAX;

	struct ActionAndTimestamp {
		uint32_t id;
		uint64_t timestamp;
		bool operator==(const ActionAndTimestamp &p_other) const { return id == p_other.id; }
	};
	LocalVector<ActionAndTimestamp> missing_actions;

	bool process_received_action(uint32_t p_action_id);
	void check_missing_actions_and_clean_up();
};

#endif
