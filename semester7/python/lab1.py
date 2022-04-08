from enum import Enum
import random


import pygame
from pygame import Vector2
from pygame.locals import *



class GameState(Enum):
    DRAWING = 1,
    PLAYING = 2,
    END = 3


class Game:
    state = GameState.DRAWING
    path = []
    path_balls = []
    shot_balls = []
    player = None

    @staticmethod
    def lose():
        pygame.display.set_caption("YOU LOSE")
        Game.state = GameState.END

    @staticmethod
    def win():
        pygame.display.set_caption("YOU WON")
        Game.state = GameState.END


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

    def touches(self, other, radius_extra=0):
        if isinstance(other, Ball):
            return (self.pos - other.pos).length_squared() < (self.radius + other.radius + radius_extra) ** 2
        elif isinstance(other, Vector2):
            return (self.pos - other).length_squared() < (self.radius + radius_extra) ** 2


class PathBall(Ball):
    def __init__(self, color, pos, path_index=0):
        super().__init__(color, pos)
        self.path_index = path_index
        self.dead = False
        self.velocity = Vector2()

    def _move_forward(self):
        if self.path_index + 1 >= len(Game.path):
            Game.lose()
            return False

        next_p = Game.path[self.path_index + 1]
        self.velocity = (next_p - self.pos).normalize() * 0.5
        self.pos += self.velocity

        if self.touches(next_p):
            self.path_index += 1
        return True

    def update(self):
        path_balls_index = Game.path_balls.index(self)
        # Last ball always moves, other balls have to be pushed forward
        if path_balls_index == len(Game.path_balls) - 1:
            self._move_forward()
        else:
            prev_ball = Game.path_balls[path_balls_index + 1]
            # Up to 5 extra moves in case of push
            i = 0
            while self.touches(prev_ball):
                if i >= 5:
                    break
                i += 1
                if not self._move_forward():
                    break

            # First ball checks for combo
            if path_balls_index == 0 or Game.path_balls[path_balls_index - 1].color != self.color:
                combo_balls = [self]
                while self.color == prev_ball.color and prev_ball.touches(combo_balls[-1], 2):
                    combo_balls.append(prev_ball)
                    prev_index = path_balls_index + len(combo_balls)
                    if prev_index == len(Game.path_balls):
                        break
                    prev_ball = Game.path_balls[prev_index]

                if len(combo_balls) >= 3:
                    for ball in combo_balls: 
                        ball.dead = True


class ShootBall(Ball):
    def __init__(self, color, pos, velocity):
        super().__init__(color, pos)
        self.velocity = velocity
        self.dead = False

    def update(self):
        self.pos += self.velocity
        i = 0
        for ball in Game.path_balls:
            if self.touches(ball):
                # Did we land in front of the ball?
                angle = ball.velocity.angle_to(self.pos - ball.pos)
                if angle > 90 or angle < -90:
                    #print("back")
                    Game.path_balls.insert(i + 1, PathBall(self.color, ball.pos, ball.path_index))
                else:
                    #print("front")
                    Game.path_balls.insert(i, PathBall(self.color, ball.pos + ball.velocity, ball.path_index))
                self.dead = True
                break
            i += 1
                


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


def main():
    # Initialise screen
    pygame.init()
    screen = pygame.display.set_mode((640, 480))
    pygame.display.set_caption('Zooma')

    # Fill background
    background = pygame.Surface(screen.get_size())
    background = background.convert()
    background.fill((255, 255, 255))

    # Initialise clock
    clock = pygame.time.Clock()

    last_colors = [BallColor.RED, BallColor.BLUE]
    balls_spawned = 0
    MAX_BALLS_SPAWN = 25

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
                if Game.state == GameState.DRAWING:
                    if event.buttons[0] == 1:
                        # LMB pressed, drawing path
                        Game.path.append(Vector2(event.pos))
                    elif Game.path:
                        # Let go of LMB, path was drawn
                        Game.state = GameState.PLAYING
            elif event.type == MOUSEBUTTONDOWN:
                mouse_down = True

        screen.blit(background, (0, 0))

        for p in Game.path:
            pygame.draw.circle(screen, (200, 200, 200), p, 5)

        if Game.state == GameState.PLAYING:
            if Game.player == None:
                Game.player = Player(BallColor.GREEN, Vector2(screen.get_size()) / 2)
            player = Game.player
            player.update()

            if mouse_down:
                shoot_velocity = Vector2()
                shoot_velocity.from_polar((5, player.rotation))
                Game.shot_balls.append(ShootBall(player.color, player.pos, shoot_velocity))

                player.color = random.choice(list(BallColor))
            for b in Game.shot_balls:
                b.update()

            if ticks % 40 == 0:
                if balls_spawned < MAX_BALLS_SPAWN:
                    balls_spawned += 1

                    # Make sure color does not match previous 2 colors
                    color = random.choice(list(BallColor))
                    while color == last_colors[0] and color == last_colors[1]:
                        color = random.choice(list(BallColor))
                    last_colors.append(color)
                    last_colors.pop(0)
                    Game.path_balls.append(PathBall(color, Game.path[0]))
            for b in Game.path_balls:
                b.update()

            # Remove 'dead' balls
            alive_balls = [b for b in Game.shot_balls if not b.dead]
            Game.shot_balls.clear()
            Game.shot_balls.extend(alive_balls)

            alive_balls = [b for b in Game.path_balls if not b.dead]
            Game.path_balls.clear()
            Game.path_balls.extend(alive_balls)

            if balls_spawned >= MAX_BALLS_SPAWN and len(Game.path_balls) == 0:
                Game.win()

            
        if Game.player:
            Game.player.draw(screen)

        for b in Game.shot_balls:
            b.draw(screen)

        for b in Game.path_balls:
            b.draw(screen)

        pygame.display.flip()


if __name__ == '__main__': main()
