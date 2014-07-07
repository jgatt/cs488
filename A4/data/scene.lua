-- A simple test scene featuring some spherical cows grazing
-- around Stonehenge.  "Assume that cows are spheres..."

stone = gr.material({0.8, 0.7, 0.7}, {0.5, 0.5, 0.5}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)

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

-- Use the instanced cow model to place some actual cows in the scene.
-- For convenience, do this in a loop.

cow_number = 1

for _, pt in pairs({
		      {{-5,1.3,-3}, 90},
		      {{0,1.3,-3}, 90},
		      {{5,1.3,-3}, 90},
		      {{-5,1.3,2}, 90},
		      {{0,1.3,2}, 90},
		      {{5,1.3,2}, 90},
		      {{-5,1.3,7}, 90},
		      {{0,1.3,7}, 90},
		      {{5,1.3,7}, 90},
		      {{-5,1.3,12}, 90},
		      {{0,1.3,12}, 90},
		      {{5,1.3,12}, 90},
		      }) do
   cow_instance = gr.node('cow' .. tostring(cow_number))
   scene:add_child(cow_instance)
   cow_instance:add_child(cow)
   cow_instance:translate(unpack(pt[1]))
   cow_instance:rotate('Y', pt[2])
   cow_instance:scale(1.4, 1.4, 1.4)
   
   cow_number = cow_number + 1
end

-- Place a ring of arches.

require('mickey')
scene:add_child(mickey)
mickey:translate(8, 2, -5)
mickey:rotate('X', -90)
mickey:rotate('Z', 90)
mickey:scale(15, 15, 15)
mickey:set_material(stone)

gr.render(scene,
	  'cow_cult.png', 1024, 1024,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0})})
