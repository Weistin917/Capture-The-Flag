#pragma once

// Spawns and owns all client-side threads (UDP discovery for now,
// TCP connection thread later). Blocks until shutdown.
void client();
