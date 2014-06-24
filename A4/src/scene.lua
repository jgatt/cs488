
stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)

--inst = gr.node('inst')

--arc = gr.node('arc')
--inst:add_child(arc)
--arc:translate(0, 0, -10)

s = gr.nh_sphere('s', {5, 5, -1}, 5)
s:set_material(stone)

--scene = gr.node('scene')
--scene:rotate('X', 23)

--scene:add_child(arc)

gr.render(s,
	  'test.png', 256, 256,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0})})