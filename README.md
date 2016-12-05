# Contrenot

The *Contrenot* (pronounced "con-truh-no") is a musical interface designed
to loosely emulate the mechanics of a bowed upright bass. It's inspiration
comes from other electronic musical interfaces such as the Ondes Martenot, 
the Gametrak (as used by SLORK), and the Omatone. The Contrenot has a form factor 
similar to that of an electric upright bass. The neck consists of a high-resolution
linear softpot and FSR for monophonic pitch and aftertouch detection. The body houses a
pull-string sensor, created from a gutted tape measure spring, a high resolution 
incremental rotary encoder, and custom 3d printed parts. The pull-string sensor is able to detect 
velocity and motion very precisely, allowing for very nuanced control similar to 
bowing a bass.
All sensors and components are powered by an Arduino Uno. Data is sent over USB serial to
a Linux computer, where it processed and mapped to sound parameters in Sporth, a 
stack-based audio synthesis language. 
