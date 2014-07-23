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

-- ##############################################
-- the arch
-- ##############################################

inst = gr.node('inst')

arc = gr.node('arc')
inst:add_child(arc)
arc:translate(0, 0, -10)

p1 = gr.nh_box('p1', {0, 0, 0}, 1)
arc:add_child(p1)
p1:set_material(stone)
p1:translate(-2.4, 0, -0.4)
p1:scale(0.8, 4, 0.8)

p2 = gr.nh_box('p2', {0, 0, 0}, 1)
arc:add_child(p2)
p2:set_material(stone)
p2:translate(1.6, 0, -0.4)
p2:scale(0.8, 4, 0.8)

s = gr.nh_sphere('s', {0, 0, 0}, 1)
arc:add_child(s)
s:set_material(stone)
s:translate(0, 4, 0)
s:scale(4, 0.6, 0.6)

-- #############################################
-- Let's assume that cows are spheres
-- #############################################

cow = gr.node('the_cow')

for _, spec in pairs({
			{'body', {0, 0, 0}, 1.0},
			{'head', {.9, .3, 0}, 0.6},
			{'tail', {-.94, .34, 0}, 0.2},
			{'lfleg', {.7, -.7, -.7}, 0.3},
			{'lrleg', {-.7, -.7, -.7}, 0.3},
			{'rfleg', {.7, -.7, .7}, 0.3},
			{'rrleg', {-.7, -.7, .7}, 0.3}
		     }) do
   part = gr.nh_sphere(unpack(spec))
   part:set_material(hide)
   cow:add_child(part)
end

-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')
scene:rotate('X', 23)

scene2 = gr.node('scene')
scene2:rotate('X', 23)

-- the floor

plane = gr.mesh('plane', {
		   { -1, 0, -1 }, 
		   {  1, 0, -1 }, 
		   {  1,  0, 1 }, 
		   { -1, 0, 1  }
		}, {
		   {3, 2, 1, 0}
		})
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)

-- Construct a central altar in the shape of a buckyball.  The
-- buckyball at the centre of the real Stonehenge was destroyed
-- in the great fire of 733 AD.

require('buckyball')

scene:add_child(buckyball)
buckyball:set_material(stone)
buckyball:scale(1.5, 1.5, 1.5)

-- Use the instanced cow model to place some actual cows in the scene.
-- For convenience, do this in a loop.

cow_number = 1

for _, pt in pairs({
		      {{1,1.3,14}, 20, {2, 0, 0}},
		      {{5,1.3,-11}, 180, {-1, 1, -1}},
		      {{-5.5,1.3,-3}, -60, {2, 1, 1}}}) do
   cow_instance = gr.node('cow' .. tostring(cow_number))
   scene:add_child(cow_instance)
   cow_instance:add_child(cow)
   cow_instance:translate(unpack(pt[1]))
   cow_instance:rotate('Y', pt[2])
   cow_instance:scale(1.4, 1.4, 1.4)
   --cow_instance:set_velocity(unpack(pt[3]))
   
   cow_number = cow_number + 1
end

-- Place a ring of arches.

for i = 1, 6 do
   an_arc = gr.node('arc' .. tostring(i))
   an_arc:rotate('Y', (i-1) * 60)
   scene:add_child(an_arc)
   an_arc:add_child(arc)
end

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
	  'simple_cows.png', 1024, 1024,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({0, 400, 200}, {0.8, 0.8, 0.8}, {1, 0, 0}, 75)})
