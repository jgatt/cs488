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
mars = gr.texture("mars.png")
brick = gr.texture("brick.png")
alice = gr.texture("space.png")

temp_scene = gr.node("scene")

alice_node = gr.node('sup')
alice_node:rotate('X', 45)
temp_scene:add_child(alice_node)

s1 = gr.nh_sphere('v', {-5, 0, 10}, 4)
temp_scene:add_child(s1)
s1:set_texture(venus, 0)

s2 = gr.nh_sphere('e', {0, 0, 0}, 4)
temp_scene:add_child(s2)
s2:set_texture(earth, 0)

s3 = gr.nh_sphere('m', {5, 0, -10}, 4)
temp_scene:add_child(s3)
s3:set_texture(mars, 0)

box = gr.nh_box('bricks', {-20, -20, -50}, 40)
temp_scene:add_child(box)
box:set_texture(alice, 0)
box:set_texture(alice, 1)
box:set_texture(alice, 2)
box:set_texture(alice, 3)
box:set_texture(alice, 4)
box:set_texture(alice, 5)

gr.render(temp_scene,
	  'depth.png', 1024, 1024,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}, 0)})
