-- A simple test scene featuring some spherical cows grazing
-- around Stonehenge.  "Assume that cows are spheres..."

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
white = gr.material({0.4, 0.4, 0.4}, {0.2, 0.2, 0.2}, 80)
orange = gr.material({1.0, 0.5, 0}, {0.2, 0.2, 0.2}, 40)
blue = gr.material({0, 1.0, 1.0}, {0.2, 0.2, 0.2}, 40)
red = gr.material({1.0, 0, 0}, {0.2, 0.2, 0.2}, 40)

earth = gr.texture("earth2.png")
venus = gr.texture("venus.png")
polka = gr.texture("polka.png")
check = gr.texture("check.png")
--mars = gr.texture("mars.png")
brick = gr.texture("brick.png")
alice = gr.texture("alice.png")

temp_scene = gr.node("scene")

alice_node = gr.node('sup')
alice_node:rotate('X', 45)
temp_scene:add_child(alice_node)

s1us2 = gr.boolean("s1s2", 1)
temp_scene:add_child(s1us2)

s1 = gr.nh_sphere('s1', {-10, 10, 0}, 2)
s1us2:add_child(s1)
s1:set_material(blue)

s2 = gr.nh_sphere('s2', {-8, 10, 0}, 2)
s1us2:add_child(s2)
s2:set_material(red)

s1is2 = gr.boolean("s1is2", 2)
temp_scene:add_child(s1is2)
s1is2:translate(8, 0, 0)
s1is2:add_child(s1)
s1is2:add_child(s2)

s1ds2 = gr.boolean("s1ds2", 3)
temp_scene:add_child(s1ds2)
s1d = gr.nh_sphere('s1d', {6, 10, 0}, 2)
s1ds2:add_child(s1d)
s1d:set_material(blue)

s2d = gr.nh_sphere('s1d', {7, 10, 1}, 2)
s1ds2:add_child(s2d)
s2d:set_material(red)

s3ub1 = gr.boolean("s1s2", 1)
temp_scene:add_child(s3ub1)

s3 = gr.nh_sphere('s1', {-8, 2, 2}, 2.4)
s3ub1:add_child(s3)
s3:set_material(blue)

b1 = gr.nh_box('b1', {-10, 0, 0}, 4)
s3ub1:add_child(b1)
b1:set_material(red)

b1us3 = gr.boolean("s1s2", 2)
b1us3:translate(8, 0, 0)
temp_scene:add_child(b1us3)
b1us3:add_child(b1)
b1us3:add_child(s3)

b1ds3 = gr.boolean("s1s2", 3)
b1ds3:translate(16, 0, 0)
temp_scene:add_child(b1ds3)
b1ds3:add_child(b1)
b1ds3:add_child(s3)

s4ub2 = gr.boolean("s1s2", 3)
temp_scene:add_child(s4ub2)

s4 = gr.nh_sphere('s1', {-8, -2.5, 3}, 2.4)
s4:set_material(blue)

b2 = gr.nh_box('b1', {-10, -6, 0}, 4)
b2:set_material(red)

s4ub2:add_child(b2)
s4ub2:add_child(s4)

alice_box = gr.nh_box('alice', {-10, -20, -10}, 20)
--alice_node:add_child(alice_box)
alice_box:set_texture(brick, 0)
alice_box:set_texture(brick, 1)
alice_box:set_texture(brick, 2)
alice_box:set_texture(brick, 3)
alice_box:set_texture(brick, 4)
alice_box:set_texture(brick, 5)

box = gr.nh_box('bricks', {-20, -20, -50}, 40)
--temp_scene:add_child(box)
box:set_texture(alice, 0)
box:set_texture(alice, 1)
box:set_texture(alice, 2)
box:set_texture(alice, 3)
box:set_texture(alice, 4)
box:set_texture(alice, 5)

gr.render(temp_scene,
	  'csg.png', 1024, 1024,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}, 0)})
