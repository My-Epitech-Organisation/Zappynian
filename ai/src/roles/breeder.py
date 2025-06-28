def breeder(queue) -> int:
    queue.send_and_wait("Fork")
    return 0
