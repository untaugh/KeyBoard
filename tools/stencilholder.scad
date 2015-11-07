/* a tool for holding the stencil in 
place while applying paste
*/

stencil_width = 52;
stencil_height = 69;
pcb_width = 12.4;
pcb_height = 29.4;
thickness = 2.5;
//thickness = 0.5; // for testing
height_offset = 0.2;

difference()
{
    cube([stencil_width,stencil_height,thickness],true);
    cutout();
}


module cutout() {
    translate([0,height_offset,0]) cube([pcb_width,pcb_height,thickness],true);
    translate([0,-20,0])
    cylinder(r=2,h=thickness,center=true);
}