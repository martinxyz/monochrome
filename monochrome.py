#!/usr/bin/env python3
import sys, time, math
import pygame, numpy
import monofast

screen_w, screen_h = 800, 600
fullscreen = False

pygame.init()
pygame.display.set_caption("Monochrome")
pygame.mouse.set_visible(False)
flags = 0
if fullscreen:
    flags |= pygame.FULLSCREEN
screen = pygame.display.set_mode((screen_w, screen_h), flags)

clock = pygame.time.Clock()

print(pygame.display.Info())
print(screen)
assert(screen.get_bitsize() == 32)
arr = pygame.surfarray.pixels2d(screen)

def quit():
    pygame.quit()
    sys.exit(0)

world = monofast.World()

i = 0
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
    #screen.fill((0,0,0))
    i += 1
    arr[:,:] = numpy.random.randint(256**3, size=(screen_w, screen_h))
    #world.radius = int(math.exp(-7 + (math.sin(i/2000*2*math.pi)+1) * 8.0))
    world.radius = int(math.exp(6.5 - math.cos(i/1000*2*math.pi) * 0.5))
    world.render(arr)
    #pygame.draw.rect(screen, (255,255,255), [10,20,30,40])
    clock.tick(60)
    #print('%.6f' % (time.time() % 1))
    pygame.display.flip()
