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

temp_scene = gr.node("scene")

alice_node = gr.node('sup')
alice_node:rotate('X', 45)
temp_scene:add_child(alice_node)

earth_node = gr.node("earth")
temp_scene:add_child(earth_node)
earth_node:rotate('X', 45)
earth_sphere = gr.nh_sphere('earth', {-4, 7, 3}, 2.5)
earth_node:add_child(earth_sphere)
earth_sphere:set_texture(earth, 0)

check_node = gr.node("check")
temp_scene:add_child(check_node)
check_node:rotate('X', 45)
check_sphere = gr.nh_sphere('check', {4, 7, 3}, 2.5)
check_node:add_child(check_sphere)
check_sphere:set_texture(venus, 0)

alice_box = gr.nh_box('alice', {-10, -20, -10}, 20)
alice_node:add_child(alice_box)
alice_box:set_texture(brick, 0)
alice_box:set_texture(brick, 1)
alice_box:set_texture(brick, 2)
alice_box:set_texture(brick, 3)
alice_box:set_texture(brick, 4)
alice_box:set_texture(brick, 5)

box = gr.nh_box('bricks', {-20, -20, -50}, 40)
temp_scene:add_child(box)
box:set_texture(alice, 0)
box:set_texture(alice, 1)
box:set_texture(alice, 2)
box:set_texture(alice, 3)
box:set_texture(alice, 4)
box:set_texture(alice, 5)

gr.render(temp_scene,
	  'soft-shadows.png', 1024, 1024,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({0, 400, 200}, {0.8, 0.8, 0.8}, {1, 0, 0}, 0)})
