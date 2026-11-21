# Private Avelorn staging

## Build a release bundle

Use JDK 25 or newer, Maven, Git, and a JVMud checkout containing the pinned revision. From the Avelorn checkout, install the pinned runtime and package a committed candidate into a new directory.

```sh
scripts/avelorn-install-jvmud
scripts/avelorn-package /tmp/avelorn-candidate <git-revision>
```

The bundle contains its commit ID in `RELEASE`, the mudlib, frozen runtime atlas data, JVMud and its runtime dependencies, and the staging tools. The packager reads the commit directly. The bundle can run on a host with JDK 25 or newer and OpenSSL. Keep the bundle and the writable stage directory separate.

## Run the staged worker

Use a short private state path so the JVMud administration socket fits the host's Unix socket path limit. Choose five consecutive free ports. The service runs in the foreground and can be supervised by the host or stopped with SIGTERM. It restarts from the same state directory and revision.

```sh
/tmp/avelorn-candidate/bin/avelorn-stage start /tmp/av-stage 45160
```

Wait for the TLS endpoint to appear in the service output. All listeners bind to loopback. The TLS endpoint uses the base port plus four. Give the game client the certificate at `/tmp/av-stage/tls.crt` as a trusted certificate for this endpoint. The operator can verify the endpoint and certificate with OpenSSL.

```sh
openssl s_client -quiet -verify_return_error -verify_ip 127.0.0.1 \
  -CAfile /tmp/av-stage/tls.crt -connect 127.0.0.1:45164
```

The `engine.log`, `engine/logs/avelorn.log`, `worker-start.log`, and `proxy.log` files are under the staging state directory. Use the owner console to inspect or restart the worker.

```sh
printf 'status\nquit\n' | /tmp/avelorn-candidate/bin/avelorn-stage admin /tmp/av-stage
```

## Verify the candidate

Run a short two-client smoke through the TLS endpoint. The harness creates isolated staging accounts, then verifies login, room descriptions, inventory, character stats, presence, and manual persistence. Clients repeatedly walk from Westwatch into the wilderness and back, checking that wilderness steps spend travel reserve and an inn rest restores it.

```sh
/tmp/avelorn-candidate/bin/avelorn-stage soak /tmp/av-stage 30 2
```

For crash recovery, first complete the smoke and capture the worker PID from the private owner console. After verifying it is the Avelorn staging worker, terminate that PID with SIGKILL, wait for the engine to report `FAILED`, then start the same port pair in the owner console. Re-run the smoke against the original stage directory. Verify returning accounts and their locations. Record the failed and running statuses and the smoke outcome.

```sh
printf 'start avelorn 45162 45163\nstatus\nquit\n' \
  | /tmp/avelorn-candidate/bin/avelorn-stage admin /tmp/av-stage
```

Run the full soak against the exact candidate revision after the smoke and separate crash rehearsal.

```sh
/tmp/avelorn-candidate/bin/avelorn-stage soak /tmp/av-stage 86400 32 \
  > /tmp/av-stage/soak.log 2>&1
```

The harness reports progress every 30 seconds and produces a final `soak_end` line. Record the staged revision, exit status, elapsed time, peak active players, verified commands, completed excursions, reserve spent, and failures. A passing run has all 32 clients playing concurrently, zero failures, and a successful exit after 24 hours.

## Promote or restore a release

1. Record the running and candidate commit IDs. Keep the verified running bundle on disk.
2. Stop the old service cleanly so active players finish saving. Back up both account checkpoint slots and retained legacy account files from `STATE/mudlibs/avelorn/accounts`. Keep the backup private and record its time.
3. Stage the candidate into its own writable state directory. Stop that staged worker and copy the reviewed account backup into its empty account store. Start the candidate and verify a returning character, one room move, and a new checkpoint.
4. To roll back, stop the candidate. Restart the previous verified bundle and state at the recorded recovery point, then verify the same checks.

Stage accounts and credentials live only in their staging state directory. Back up player account state from the service being promoted.
