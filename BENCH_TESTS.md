# Bench test checklist

Status: not performed. Complete these checks on actual hardware and record results before claiming hardware validation.

1. Inspect the disconnected circuit against the diagram, including the shared ground and servo supply polarity.
2. Test the servo without a hopper or load. Confirm startup moves to the closed position.
3. Send `s` at 9600 baud and confirm the reported count starts at zero.
4. Send `f`; check that the gate opens, LED lights and the gate closes after approximately 700 ms.
5. Press the button during an active feed and within the cooldown. Confirm neither starts another feed.
6. After cooldown, press and hold the button. Confirm one feed, with no repeated feeding while held.
7. Use the short demonstration interval described in README to check automatic feeds and confirm manual feeding restarts the timer.
8. Disconnect and reconnect power. Confirm no immediate feed and note that the timer and count reset.
9. With the real mechanism, calibrate opening angle and duration, measure repeated portions, and check for jams or resets under load.
10. Restore the normal interval and cooldown values before the final demonstration.

Record: date, hardware, firmware settings, expected result, observed result and pass/fail. Add photos or a video of the real assembly when available.
