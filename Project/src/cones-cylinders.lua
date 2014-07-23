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

cone1_node = gr.node("cone1")
temp_scene:add_child(cone1_node)
cone1_node:rotate('Z', 75)
cone1_node:rotate('X', 20)
con1 = gr.nh_cone('earth', {-4, 7, 3}, 1.5, 4)
cone1_node:add_child(con1)
con1:set_material(hide)

cone2_node = gr.node("cone2")
temp_scene:add_child(cone2_node)
cone2_node:rotate('Z', -75)
cone2_node:rotate('X', 20)
con2 = gr.nh_cone('earth', {-4, 7, 3}, 1.5, 4)
cone2_node:add_child(con2)
con2:set_material(hide)

cone3_node = gr.node("cone3")
temp_scene:add_child(cone3_node)
con3 = gr.nh_cone('earth', {-4, 7, 3}, 1.5, 4)
cone3_node:add_child(con3)
con3:set_material(hide)

cyl1_node = gr.node("cyl1")
cyl1_node:rotate('Z', 90)
temp_scene:add_child(cyl1_node)
cyl1 = gr.nh_cylinder('earth', {-2, -3.5, 3}, 1.5, 7)
cyl1_node:add_child(cyl1)
cyl1:set_material(hide)

cyl2_node = gr.node("cyl2")
temp_scene:add_child(cyl2_node)
cyl2 = gr.nh_cylinder('earth', {-5,  -3, 10}, 1.5, 10)
cyl2_node:add_child(cyl2)
cyl2:set_material(hide)

cyl3_node = gr.node("cyl3")
temp_scene:add_child(cyl3_node)
cyl3 = gr.nh_cylinder('earth', {5,  -3, 10}, 1.5, 10)
cyl3_node:add_child(cyl3)
cyl3:set_material(hide)

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
	  'cones-cylinders.png', 1024, 1024,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0}, 0)})
