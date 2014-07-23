-- A simple test scene featuring some spherical cows grazing
-- around Stonehenge.  "Assume that cows are spheres..."

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
white = gr.material({0.4, 0.4, 0.4}, {0.2, 0.2, 0.2}, 80)
orange = gr.material({1.0, 0.5, 0}, {0.2, 0.2, 0.2}, 40)
blue = gr.material({0, 1.0, 1.0}, {0.2, 0.2, 0.2}, 40)
red = gr.material({1.0, 0, 0}, {0.2, 0.2, 0.2}, 40)

grass = gr.texture("grass.png") 
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

alice_box = gr.nh_box('alice', {-18, -35, -10}, 35)
alice_node:add_child(alice_box)
alice_box:set_texture(grass, 0)
alice_box:set_texture(grass, 1)
alice_box:set_texture(grass, 2)
alice_box:set_texture(grass, 3)
alice_box:set_texture(grass, 4)
alice_box:set_texture(grass, 5)

box = gr.nh_box('bricks', {-20, -20, -50}, 40)
temp_scene:add_child(box)
box:set_texture(alice, 0)
box:set_texture(alice, 1)
box:set_texture(alice, 2)
box:set_texture(alice, 3)
box:set_texture(alice, 4)
box:set_texture(alice, 5)

gr.render(temp_scene,
	  'anti.png', 1024, 1024,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}, 0)})
