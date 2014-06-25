
stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)

--inst = gr.node('inst')

--arc = gr.node('arc')
--inst:add_child(arc)
--arc:translate(0, 0, -10)

-- COWS START
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
-- COWS END

scene = gr.node('scene')

s = gr.nh_sphere('s', {10, 10, -1}, 5)
scene:add_child(s)
s:set_material(stone)

scene2 = gr.node("scene2")
scene:add_child(scene2)

s2 = gr.nh_sphere('s2', {10, 1, -2}, 5)
scene2:add_child(s2)
s2:set_material(hide)

s3 = gr.nh_sphere('s3', {0, 0, -3}, 5)
scene2:add_child(s3)
s3:set_material(grass)

sbox = gr.nh_box('sbox', {-10, 4, -2}, 8)
scene2:add_child(sbox)
sbox:set_material(grass)

--ALL THE COWS START
scene_cow = gr.node('scow')
scene:add_child(scene_cow)
cow_number = 1

for _, pt in pairs({
		      {{1,1.3,14}, 0},
		      {{5,1.3,-11}, 30},
		      {{-5.5,1.3,-3}, 20}}) do
   cow_instance = gr.node('cow' .. tostring(cow_number))
   scene_cow:add_child(cow_instance)
   cow_instance:add_child(cow)
   cow_instance:translate(unpack(pt[1]))
   cow_instance:rotate('Y', pt[2])
   cow_instance:scale(1.8, 1.8, 1.8)
   
   cow_number = cow_number + 1
end
--ALL THE COWS END


scene_s = gr.node('sphere')
scene_s:rotate('Y', 50)

ss = gr.nh_sphere('ss', {10, 1, -2}, 5)
scene_s:add_child(ss)
ss:set_material(hide)

gr.render(scene_cow,
	  'test.png', 256, 256,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0})})
