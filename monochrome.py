#!/usr/bin/env python3
import sys, time, math, random
import pygame, numpy
import monofast

screen_w, screen_h = 800, 600
fullscreen = False

pygame.init()
pygame.display.set_caption("Monochrome")
#pygame.mouse.set_visible(False)
flags = 0
#flags |= pygame.DOUBLEBUF | pygame.HWSURFACE
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
t0 = time.time()
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            quit()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            print('button')
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                quit()
        elif event.type == pygame.MOUSEMOTION:
            x, y = pygame.mouse.get_pos()
            world.dab(x*4 + random.choice((0,1,2,3)), y*4+random.choice((0,1,2,3)), 70)
        #pos = pygame.mouse.get_pos()
    #screen.fill((0,0,0))
    N=64
    if i % N == 0:
        print("%.3f FPS" % (N/(time.time()-t0)))
        t0 = time.time()
    i += 1
    #arr[:,:] = numpy.random.randint(256**3, size=(screen_w, screen_h))
    for j in range(16):
        world.move()
        world.expose()
    world.render(arr)
    #pygame.draw.rect(screen, (255,255,255), [10,20,30,40])
    clock.tick(30)
    #print('%.6f' % (time.time() % 1))
    pygame.display.flip()
