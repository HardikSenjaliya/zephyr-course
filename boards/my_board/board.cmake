# Even if flash and start work, dfu-util return error 74. It can be ignored.
board_runner_args(dfu-util "--pid=0483:df11" "--alt=0" "--dfuse")

# keep first
include(${ZEPHYR_BASE}/boards/common/dfu-util.board.cmake)
