/**
 * Copyright (c) 2020 Paul-Louis Ageneau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ice_agent.h"
#include "juice/juice.h"
#include "log.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
static void sleep(unsigned int secs) { Sleep(secs * 1000); }
#else
#include <unistd.h> // for sleep
#endif

#define BUFFER_SIZE 4096

static juice_agent_t *agent1;
static juice_agent_t *agent2;

static IceAgent ice_agent1;
static IceAgent ice_agent2;

static void on_state_changed1(juice_agent_t *agent, juice_state_t state, void *user_ptr);
static void on_state_changed2(juice_agent_t *agent, juice_state_t state, void *user_ptr);

static void on_candidate1(juice_agent_t *agent, const char *sdp, void *user_ptr);
static void on_candidate2(juice_agent_t *agent, const char *sdp, void *user_ptr);

static void on_gathering_done1(juice_agent_t *agent, void *user_ptr);
static void on_gathering_done2(juice_agent_t *agent, void *user_ptr);

static void on_recv1(juice_agent_t *agent, const char *data, size_t size, void *user_ptr);
static void on_recv2(juice_agent_t *agent, const char *data, size_t size, void *user_ptr);

int main()
{
	juice_set_log_level(JUICE_LOG_LEVEL_DEBUG);

	// Agent 1: Create agent
	ice_agent1.CreateIceAgent(
		on_state_changed1,
		on_candidate1,
		on_gathering_done1,
		on_recv1,
		nullptr);

	// Agent 2: Create agent
	ice_agent2.CreateIceAgent(
		on_state_changed2,
		on_candidate2,
		on_gathering_done2,
		on_recv2,
		nullptr);

	// Agent 1: Generate local description
	char *local_sdp1 = ice_agent1.GenerateLocalSdp();

	// Agent 2: Receive description from agent 1
	ice_agent2.SetRemoteSdp(local_sdp1);

	// Agent 2: Generate local description
	char *local_sdp2 = ice_agent2.GenerateLocalSdp();

	// Agent 1: Receive description from agent 2
	ice_agent1.SetRemoteSdp(local_sdp2);

	// Agent 1: Gather candidates (and send them to agent 2)
	ice_agent1.GatherCandidates();
	sleep(2);

	// Agent 2: Gather candidates (and send them to agent 1)
	ice_agent2.GatherCandidates();
	sleep(2);

	// -- Connection should be finished --

	// Check states
	juice_state_t state1 = ice_agent1.GetIceState();
	juice_state_t state2 = ice_agent2.GetIceState();
	bool success = (state1 == JUICE_STATE_COMPLETED &&
					state2 == JUICE_STATE_COMPLETED);

	// Retrieve candidates
	success = ice_agent1.GetSelectedCandidates();
	success = ice_agent2.GetSelectedCandidates();

	// Retrieve addresses
	success = ice_agent1.GetSelectedAddresses();
	success = ice_agent2.GetSelectedAddresses();

	// Agent 1: destroy
	ice_agent1.DestoryIceAgent();

	// Agent 2: destroy
	ice_agent2.DestoryIceAgent();

	// Sleep so we can check destruction went well
	sleep(2);

	if (success)
	{
		LOG_INFO("Success");
		return 0;
	}
	else
	{
		LOG_INFO("Failure");
		return -1;
	}
}

// Agent 1: on state changed
static void on_state_changed1(juice_agent_t *agent, juice_state_t state, void *user_ptr)
{
	LOG_INFO("State 1: {}", juice_state_to_string(state));

	if (state == JUICE_STATE_CONNECTED)
	{
		// Agent 1: on connected, send a message
		const char *message = "Hello from 1";
		ice_agent1.Send(message);
	}
}

// Agent 2: on state changed
static void on_state_changed2(juice_agent_t *agent, juice_state_t state, void *user_ptr)
{
	LOG_INFO("State 2: {}", juice_state_to_string(state));
	if (state == JUICE_STATE_CONNECTED)
	{
		// Agent 2: on connected, send a message
		const char *message = "Hello from 2";
		ice_agent2.Send(message);
	}
}

// Agent 1: on local candidate gathered
static void on_candidate1(juice_agent_t *agent, const char *sdp, void *user_ptr)
{
	LOG_INFO("Candidate 1: {}", sdp);

	// Agent 2: Receive it from agent 1
	ice_agent2.AddRemoteCandidates(sdp);
}

// Agent 2: on local candidate gathered
static void on_candidate2(juice_agent_t *agent, const char *sdp, void *user_ptr)
{
	LOG_INFO("Candidate 2: {}", sdp);

	// Agent 1: Receive it from agent 2
	ice_agent1.AddRemoteCandidates(sdp);
}

// Agent 1: on local candidates gathering done
static void on_gathering_done1(juice_agent_t *agent, void *user_ptr)
{
	LOG_INFO("Gathering done 1");
	ice_agent2.SetRemoteGatheringDone(); // optional
}

// Agent 2: on local candidates gathering done
static void on_gathering_done2(juice_agent_t *agent, void *user_ptr)
{
	LOG_INFO("Gathering done 2");
	ice_agent1.SetRemoteGatheringDone(); // optional
}

// Agent 1: on message received
static void on_recv1(juice_agent_t *agent, const char *data, size_t size, void *user_ptr)
{
	char buffer[BUFFER_SIZE];
	if (size > BUFFER_SIZE - 1)
		size = BUFFER_SIZE - 1;
	memcpy(buffer, data, size);
	buffer[size] = '\0';
	LOG_INFO("Received 1: {}", buffer);
}

// Agent 2: on message received
static void on_recv2(juice_agent_t *agent, const char *data, size_t size, void *user_ptr)
{
	char buffer[BUFFER_SIZE];
	if (size > BUFFER_SIZE - 1)
		size = BUFFER_SIZE - 1;
	memcpy(buffer, data, size);
	buffer[size] = '\0';
	LOG_INFO("Received 2: {}", buffer);
}
