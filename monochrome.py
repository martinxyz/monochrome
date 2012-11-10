#!/usr/bin/env python3
import sys, time
import pygame, numpy

screen_w, screen_h = 1024, 1024

pygame.init()
pygame.display.set_caption("My Game")
screen = pygame.display.set_mode((screen_w, screen_h))
clock = pygame.time.Clock()

#print(pygame.display.Info())
assert(screen.get_bitsize() == 32)
print(screen)
arr = pygame.surfarray.pixels2d(screen)

def quit():
    pygame.quit()
    sys.exit(0)

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            quit()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            print('button')
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                quit()
        #pos = pygame.mouse.get_pos()
    screen.fill((0,0,0))
    arr[:,:] = numpy.random.randint(256**3, size=(screen_w, screen_h))
    pygame.draw.rect(screen, (255,255,255), [10,20,30,40])
    clock.tick(20)
    #print('%.6f' % (time.time() % 1))
    pygame.display.flip()
