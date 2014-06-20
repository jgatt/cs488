--
-- CS488 -- Introduction to Computer Graphics
-- 
-- a3mark.lua
--
-- A very simple scene creating a trivial puppet.  The TAs will be
-- using this scene as part of marking your assignment.  You'll
-- probably want to make sure you get reasonable results with it!

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

s0 = gr.sphere('Torso')
rootnode:add_child(s0)
s0:scale(1.2, 1.7, 1);
s0:set_material(white)

shnode = gr.node("Shoulder Node")
rootnode:add_child(shnode)
shnode:translate(0, 1.4, 0)

hinode = gr.node("Hips Node")
rootnode:add_child(hinode)
hinode:translate(0, -1.4, 0)

-- HEAD STUFF START
nb = gr.joint("Neck Bottom", {-75.0, 0, 75.0}, {-90.0, 0, 90.0})
shnode:add_child(nb)
nb:translate(0, 0.4, 0)

n = gr.sphere("Neck")
nb:add_child(n)
n:scale(0.3, 0.4, 1)
n:set_material(red)

nt = gr.joint("Neck top", {-25.0, 0, 25.0}, {-45.0, 0, 45.0})
nb:add_child(nt)
nt:translate(0, 0.8, 0)

head = gr.sphere('Head')
nt:add_child(head)
head:scale(0.9, 0.9, 1)
head:set_material(white)

nose = gr.sphere("Nose")
nt:add_child(nose)
nose:scale(0.2, 0.2, 1)
nose:translate(0, 0, 1)
nose:set_material(green)
-- HEAD STUFF END

--RIGHT ARM START
rasj = gr.joint("Right Arm SJ", {0, 180.0, 250.0}, {0, 0, 45.0})
shnode:add_child(rasj)
rasj:translate(-1.8, -0.5, 0)
rasj:rotate('x', 180.0)

rabicep = gr.sphere("Right Arm Bicep")
rasj:add_child(rabicep)
rabicep:scale(0.4, 0.8, 1)
rabicep:set_material(blue)

raelbow = gr.joint("Right Arm Elbow", {-80.0, 0, 90.0}, {0, 0, 45.0})
rasj:add_child(raelbow)
raelbow:translate(0, 1.0, 0)

raforearm = gr.sphere("Right Arm Forearm")
raelbow:add_child(raforearm)
raforearm:scale(0.3, 0.8, 1)
raforearm:set_material(red)

rahj = gr.joint("Right Arm Hand Joint", {-25.0, 0, 25.0}, {0, 0, 45.0})
raelbow:add_child(rahj)
rahj:translate(0, 0.8, 0)

rhand = gr.sphere("Right Hand")
rahj:add_child(rhand)
rhand:scale(0.4, 0.4, 1)
rhand:set_material(white)
--RIGHT ARM END

--LEFT ARM START
lasj = gr.joint("Left Arm SJ", {0, 180.0, 250.0}, {-45.0, 0, 45.0})
shnode:add_child(lasj)
lasj:translate(1.8, -0.5, 0)
lasj:rotate('x', 180.0)

labicep = gr.sphere("Left Arm Bicep")
lasj:add_child(labicep)
labicep:scale(0.4, 0.8, 1)
labicep:set_material(blue)

laelbow = gr.joint("Left Arm Elbow", {-80.0, 0, 90.0}, {-45.0, 0, 45.0})
lasj:add_child(laelbow)
laelbow:translate(0, 1.0, 0)

laforearm = gr.sphere("Left Arm Forearm")
laelbow:add_child(laforearm)
laforearm:scale(0.3, 0.8, 1)
laforearm:set_material(red)

lahj = gr.joint("Left Arm Hand Joint", {-25.0, 0, 25.0}, {-45.0, 0, 45.0})
laelbow:add_child(lahj)
lahj:translate(0, 0.8, 0)

lhand = gr.sphere("Left Hand")
lahj:add_child(lhand)
lhand:scale(0.4, 0.4, 1)
lhand:set_material(white)
--LEFT ARM END

--RIGHT LEG START
rltj = gr.joint("Right Leg Thigh Joint", {90.0, 180.0, 275.0}, {0, 0, 45.0})
hinode:add_child(rltj)
rltj:translate(-1.0, -0.5, 0)
rltj:rotate('x', 180.0)

rlthigh = gr.sphere("Right Leg Thigh")
rltj:add_child(rlthigh)
rlthigh:scale(0.4, 0.8, 1)
rlthigh:set_material(blue)

rlknee = gr.joint("Right Leg Knee", {-80.0, 0, 90.0}, {0, 0, 45.0})
rltj:add_child(rlknee)
rlknee:translate(0, 1.0, 0)

rlcalf = gr.sphere("Right Leg Calf")
rlknee:add_child(rlcalf)
rlcalf:scale(0.3, 0.8, 1)
rlcalf:set_material(red)

rlankle = gr.joint("Right Leg Ankle", {-25.0, 0, 25.0}, {0, 0, 45.0})
rlknee:add_child(rlankle)
rlankle:translate(0, 0.8, 0)

rlfoot = gr.sphere("Right Leg Foot")
rlankle:add_child(rlfoot)
rlfoot:scale(0.4, 0.4, 1)
rlfoot:set_material(white)
--RIGHT LEG END

--LEFT LEG START
lltj = gr.joint("Left Leg Thigh Joint", {90.0, 180.0, 275.0}, {0, 0, 45.0})
hinode:add_child(lltj)
lltj:translate(1.0, -0.5, 0)
lltj:rotate('x', 180.0)

llthigh = gr.sphere("Left Leg Thigh")
lltj:add_child(llthigh)
llthigh:scale(0.4, 0.8, 1)
llthigh:set_material(blue)

llknee = gr.joint("Left Leg Knee", {-80.0, 0, 90.0}, {0, 0, 45.0})
lltj:add_child(llknee)
llknee:translate(0, 1.0, 0)

llcalf = gr.sphere("Left Leg Calf")
llknee:add_child(llcalf)
llcalf:scale(0.3, 0.8, 1)
llcalf:set_material(red)

llankle = gr.joint("Left Leg Ankle", {-25.0, 0, 25.0}, {0, 0, 45.0})
llknee:add_child(llankle)
llankle:translate(0, 0.8, 0)

llfoot = gr.sphere("Left Leg Foot")
llankle:add_child(llfoot)
llfoot:scale(0.4, 0.4, 1)
llfoot:set_material(white)
--LEFT LEG END

--Hips and Shoulders Start
shsphere = gr.sphere('Shoulders')
shnode:add_child(shsphere)
shsphere:scale(2.0, 0.5, 1)
shsphere:set_material(white)

hisphere = gr.sphere('Hips')
hinode:add_child(hisphere)
hisphere:scale(1.2, 0.5, 1)
hisphere:set_material(white)
--Hips and Shoulders End

rootnode:translate(0, 0, -12.0)

return rootnode
