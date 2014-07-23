-- A simple test scene featuring some spherical cows grazing
-- around Stonehenge.  "Assume that cows are spheres..."

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
white = gr.material({0.4, 0.4, 0.4}, {0.2, 0.2, 0.2}, 80)
earth = gr.texture("earth2.png")
venus = gr.texture("venus.png")
polka = gr.texture("polka.png")
check = gr.texture("check.png")
--mars = gr.texture("mars.png")
brick = gr.texture("brick.png")
alice = gr.texture("alice.png")

vanposz = gr.texture('posz.png')
vannegz = gr.texture('negz.png')
vanposx = gr.texture('posx.png')
vannegx = gr.texture('negx.png')
vanposy = gr.texture('posy.png')
vannegy = gr.texture('negy.png')

temp_scene = gr.node("scene")

alice_node = gr.node('sup')
alice_node:rotate('X', 45)
temp_scene:add_child(alice_node)

v1 = gr.nh_box('v1', {0, 6, 0}, 4)
v1:set_texture(vanposz, 0)
v1:set_texture(vanposx, 1)
v1:set_texture(vannegx, 2)
v1:set_texture(vannegz, 3)
v1:set_texture(vanposy, 4)
v1:set_texture(vannegy, 5)

v1_node = gr.node('v1n')
temp_scene:add_child(v1_node)
--v1_node:rotate('X', 25)
v1_node:translate(2, -8, 9)
v1_node:rotate('Y', 45)
--v1_node:translate(-12, 0, 5)
v1_node:add_child(v1)

v1_node2 = gr.node('v1n')
temp_scene:add_child(v1_node2)
--v1_node2:rotate('X', 25)
v1_node2:translate(-3, -8, 12)
v1_node2:rotate('Y', 135)
v1_node2:add_child(v1)

v1_node3 = gr.node('v1n')
temp_scene:add_child(v1_node3)
--v1_node2:rotate('X', 25)
v1_node3:translate(8, -2, 7)
v1_node3:rotate('Y', 225)
v1_node3:add_child(v1)

v1_node4 = gr.node('v1n')
temp_scene:add_child(v1_node4)
--v1_node2:rotate('X', 25)
v1_node4:translate(-3, -2, 6)
v1_node4:rotate('Y', 315)
v1_node4:add_child(v1)

alice_box = gr.nh_box('alice', {-10, -20, -10}, 20)
alice_node:add_child(alice_box)
alice_box:set_texture(brick, 0)
alice_box:set_texture(brick, 1)
alice_box:set_texture(brick, 2)
alice_box:set_texture(brick, 3)
alice_box:set_texture(brick, 4)
alice_box:set_texture(brick, 5)

box = gr.nh_box('bricks', {-20, -19, -50}, 40)
temp_scene:add_child(box)
box:set_texture(alice, 0)
box:set_texture(alice, 1)
box:set_texture(alice, 2)
box:set_texture(alice, 3)
box:set_texture(alice, 4)
box:set_texture(alice, 5)

gr.render(temp_scene,
	  'texture.png', 1024, 1024,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}, 0)})
