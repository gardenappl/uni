VERSION = "0.4"

from enum import Enum
import random


import pygame
from pygame import Vector2
from pygame.locals import *


def lose():
    pygame.display.set_caption("YOU LOSE")
    global state
    state = GameState.END


class BallColor(Enum):
    RED = (255, 100, 100),
    GREEN = (100, 255, 100),
    BLUE = (100, 100, 255)


class Ball:
    def __init__(self, color, pos):
        self.color = color
        self.pos = Vector2(pos)
        self.radius = 10

    def draw(self, surface):
        pygame.draw.circle(surface, self.color.value, self.pos, self.radius)

    def touches(self, other):
        if isinstance(other, Ball):
            return (self.pos - other.pos).length_squared() < (self.radius + other.radius) ** 2
        elif isinstance(other, Vector2):
            return (self.pos - other).length_squared() < self.radius ** 2


class PathBall(Ball):
    def __init__(self, color, path, path_balls, path_balls_index):
        super().__init__(color, path[0])
        self.path = path
        self.path_index = 0

        self.path_balls = path_balls
        self.path_balls_index = path_balls_index
        self.velocity = None

    def _move_forward(self):
        if self.path_index + 1 >= len(self.path):
            lose()
            return False

        next_p = self.path[self.path_index + 1]
        self.velocity = (next_p - self.pos).normalize()
        self.pos += self.velocity

        if self.touches(next_p):
            self.path_index += 1
        return True

    def update(self):
        self._move_forward()


class ShootBall(Ball):
    def __init__(self, color, pos, velocity):
        super().__init__(color, pos)
        self.velocity = velocity

    def update(self):
        self.pos += self.velocity


class Player():
    def __init__(self, color, pos):
        self.color = color
        self.rotation = 0
        self.pos = pos

    def update(self):
        mouse_pos = Vector2(pygame.mouse.get_pos())
        self.rotation = (mouse_pos - self.pos).as_polar()[1]

    def draw(self, surface):
        base_triangle = [Vector2(-20, 10), Vector2(-20, -10), Vector2(20, 0)]
        triangle = []
        for p in base_triangle:
            p = p.rotate(self.rotation)
            p += self.pos
            triangle.append(p)

        pygame.draw.polygon(surface, self.color.value, triangle)


class GameState(Enum):
    DRAWING = 1,
    PLAYING = 2,
    END = 3


def main():
    # Initialise screen
    pygame.init()
    screen = pygame.display.set_mode((640, 480))
    pygame.display.set_caption('Zooma')

    # Fill background
    background = pygame.Surface(screen.get_size())
    background = background.convert()
    background.fill((255, 255, 255))


    global state
    state = GameState.DRAWING

    path = []
    path_balls = []
    shot_balls = []
    player = None

    # Initialise clock
    clock = pygame.time.Clock()

    # Event loop
    ticks = 0
    while True:
        # Make sure game doesn't run at more than 60 frames per second
        clock.tick(60)

        ticks += 1

        mouse_down = False

        for event in pygame.event.get():
            if event.type == QUIT:
                return
            elif event.type == MOUSEMOTION:
                if state == GameState.DRAWING:
                    if event.buttons[0] == 1:
                        # LMB pressed, drawing path
                        path.append(Vector2(event.pos))
                    elif path:
                        # Let go of LMB, path was drawn
                        state = GameState.PLAYING
                        
                        print(path)
            elif event.type == MOUSEBUTTONDOWN:
                mouse_down = True

        screen.blit(background, (0, 0))

        for p in path:
            pygame.draw.circle(screen, (200, 200, 200), p, 5)

        if state == GameState.PLAYING:
            if player == None:
                player = Player(BallColor.GREEN, Vector2(screen.get_size()) / 2)
            player.update()

            if mouse_down:
                shoot_velocity = Vector2()
                shoot_velocity.from_polar((2, player.rotation))
                shot_balls.append(ShootBall(player.color, player.pos, shoot_velocity))

                player.color = random.choice(list(BallColor))
            for b in shot_balls:
                b.update()

            if ticks % 20 == 0:
                path_balls.append(PathBall(BallColor.BLUE, path, path_balls, len(path_balls)))
            for b in path_balls:
                b.update()

            
        if player:
            player.draw(screen)

        for b in shot_balls:
            b.draw(screen)

        for b in path_balls:
            b.draw(screen)


#        screen.blit(background, ball.rect, ball.rect)
#        screen.blit(background, player1.rect, player1.rect)
#        screen.blit(background, player2.rect, player2.rect)
#        ballsprite.update()
#        playersprites.update()
#        ballsprite.draw(screen)
#        playersprites.draw(screen)
        pygame.display.flip()


if __name__ == '__main__': main()
