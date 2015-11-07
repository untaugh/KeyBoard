/* a tool for holding the stencil in 
place while applying paste
*/

stencil_width = 52;
stencil_height = 69;
pcb_width = 12;
pcb_height = 29;
thickness = 2.5;
//height_offset = 2.5;
height_offset = 0.5; // for testing

difference()
{
    cube([stencil_width,stencil_height,thickness],true);
    
    translate([0,height_offset,0]) cube([pcb_width,pcb_height,thickness],true);
}