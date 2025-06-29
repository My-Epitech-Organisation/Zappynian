from ai.src.utils.route_factory import route_to

def survivor(queue, vision) -> int:
    food_tile = vision.find_nearest("food")
    if food_tile != -1:
        path = route_to(food_tile)
        for cmd in path:
            queue.send_and_wait(cmd)
        queue.send_and_wait("Take food")
        return 0
    else:
        return 1
