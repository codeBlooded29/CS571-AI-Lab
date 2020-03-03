import pygame
import sys
from pygame.locals import *
from math import sin, cos, radians
from fuzzylogic import compute

pygame.init()

WINDOWSIZE = 700
TIMETICK = 1
BOBSIZE = 15
e_theta = [3, 2, 5]
e_omega = [2, 2, 4]
e_curr = [1, 2, 4, 3, 5, 6]

window = pygame.display.set_mode((WINDOWSIZE, WINDOWSIZE))
pygame.display.set_caption("Inverted Pendulum Fuzzy Logic")

screen = pygame.display.get_surface()
screen.fill((0, 153, 255))

PIVOT = (WINDOWSIZE // 2, 9 * WINDOWSIZE // 10)
SWINGLENGTH = 320


class BobMass(pygame.sprite.Sprite):
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.theta = 1
        self.dtheta = -2.5
        self.rect = pygame.Rect(int(PIVOT[0] - SWINGLENGTH * cos(self.theta)),
                                int(PIVOT[1] - SWINGLENGTH * sin(self.theta)),
                                1, 1)
        self.draw()

    def recomputeAngle(self):

        current = compute(
            self.theta, self.dtheta, e_theta, e_omega, e_curr)
        t = TIMETICK/450
        theta_new = self.theta + self.dtheta*t + current*(t**2)*0.5
        omega_new = self.dtheta + current*t
        self.theta, self.dtheta = theta_new, omega_new
        print(theta_new, omega_new)

        self.rect = pygame.Rect(PIVOT[0] -
                                SWINGLENGTH * sin(self.theta),
                                PIVOT[1] -
                                SWINGLENGTH * cos(self.theta), 1, 1)

    def draw(self):
        pygame.draw.circle(screen, (255, 255, 255), PIVOT, 5, 0)
        pygame.draw.circle(screen, (255, 255, 255),
                           self.rect.center, BOBSIZE, 0)
        pygame.draw.aaline(screen, (255, 255, 255), PIVOT, self.rect.center)
        pygame.draw.line(screen, (255, 255, 255),
                         (0, PIVOT[1]), (WINDOWSIZE, PIVOT[1]))

    def update(self):
        self.recomputeAngle()
        screen.fill((0, 153, 255))
        self.draw()


bob = BobMass()
clock = pygame.time.Clock()


TICK = USEREVENT
pygame.time.set_timer(TICK, TIMETICK)


def input(events):
    for event in events:
        if event.type == QUIT:
            sys.exit(0)
        elif event.type == TICK:
            bob.update()


while True:
    input(pygame.event.get())
    pygame.display.flip()
