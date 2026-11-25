// HomeSense enclosure — editable OpenSCAD prototype
// Units: millimeters. Preliminary fit dimensions; verify against the actual modules before printing.

$fn = 64;

// Main enclosure envelope
case_x = 100;
case_y = 70;
case_z = 26;
wall = 2.4;
roof = 2.6;
corner_r = 4;

// Front display aperture (clear opening)
display_x = 43.5;
display_y = 28.5;
display_center_y = 8;
display_corner_r = 1.5;

// Waveshare 1.9-inch module retention (board is approx. 51 x 34 mm)
display_board_x = 51;
display_board_y = 34;
retainer_outer_x = 58;
retainer_outer_y = 43;
retainer_inner_x = display_x;
retainer_inner_y = display_y;
retainer_thickness = 1.6;
retainer_screw_d = 2.2;
retainer_boss_d = 5;
retainer_screw_x = 27.5;
retainer_screw_y = 19.5;

// Three BME280 airflow slits on the right side wall
vent_x = 16;
vent_z = 1.6;
vent_spacing = 4;
vent_center_y = -20;
vent_center_z = 16;
vent_count = 3;

// USB opening on the front short wall (Micro-USB assumption)
usb_opening_x = 12;
usb_opening_z = 7;
usb_center_x = 0;
usb_center_z = 13;

// Pushbutton access opening on the top face, left of the display
button_hole_d = 6;
button_center_x = 0;
button_center_y = -13.5;

// Indicator LED opening on the top face, to the right of the display
led_hole_d = 5.2;
led_seat_d = 6.5;
led_seat_depth = 0.7;
led_center_x = 32;
led_center_y = 8;

// Raised product label on the top face, below the display opening
label_text = "HomeSense";
label_size = 6;
label_height = 0.7;
label_center_y = -24;

// Four wall-mount screw holes and reinforced internal bosses
mount_inset_x = 9;
mount_inset_y = 9;
screw_d = 4.4;           // clearance for common M4 / #8 mounting screws
boss_d = 11;
boss_height = case_z - roof;
countersink_d = 8.0;
countersink_depth = 1.2;

// Rounded rectangular prism centered in XY, with its bottom at z=0.
module rounded_prism(x, y, z, r) {
    linear_extrude(height=z)
        offset(r=r)
            square([x-2*r, y-2*r], center=true);
}

module enclosure() {
    difference() {
        union() {
            // Open-back shell: roof and perimeter walls remain after cavity subtraction.
            rounded_prism(case_x, case_y, case_z, corner_r);

            // Reinforced screw bosses inside the shell, fused to the roof.
            for (sx = [-1, 1], sy = [-1, 1]) {
                translate([sx*(case_x/2-mount_inset_x), sy*(case_y/2-mount_inset_y), wall])
                    cylinder(d=boss_d, h=boss_height-wall);
            }

            // Small posts around the display hold the separate rear retaining frame.
            for (sx = [-1, 1], sy = [-1, 1]) {
                translate([sx*retainer_screw_x, display_center_y+sy*retainer_screw_y, case_z-roof-3.2])
                    cylinder(d=retainer_boss_d, h=3.2);
            }
        }

        // Main open underside cavity. Leaves the roof thickness intact.
        translate([0, 0, -0.1])
            rounded_prism(case_x-2*wall, case_y-2*wall, case_z-roof+0.1, max(0.5,corner_r-wall));

        // Display opening through the roof.
        translate([0, display_center_y, case_z-roof-0.1])
            rounded_prism(display_x, display_y, roof+0.2, display_corner_r);

        // Three narrow airflow slits through the right side wall.
        // Position the BME280 breakout inside behind these openings.
        for (i = [0:vent_count-1]) {
            z = vent_center_z + (i-(vent_count-1)/2)*vent_spacing;
            translate([case_x/2-wall/2, vent_center_y, z])
                cube([wall+0.2, vent_x, vent_z], center=true);
        }

        // USB connector opening through the front short wall.
        translate([usb_center_x, -case_y/2+wall/2, usb_center_z])
            cube([usb_opening_x, wall+0.2, usb_opening_z], center=true);

        // Pushbutton access hole through the top face.
        translate([button_center_x, button_center_y, case_z-roof-0.1])
            cylinder(d=button_hole_d, h=roof+0.2);

        // Indicator LED hole and shallow rim seat through the top face.
        translate([led_center_x, led_center_y, case_z-roof-0.1])
            cylinder(d=led_hole_d, h=roof+0.2);
        translate([led_center_x, led_center_y, case_z-led_seat_depth])
            cylinder(d=led_seat_d, h=led_seat_depth+0.1);

        // Blind pilot holes for the display retaining-frame screws.
        for (sx = [-1, 1], sy = [-1, 1]) {
            translate([sx*retainer_screw_x, display_center_y+sy*retainer_screw_y, case_z-roof-3.25])
                cylinder(d=1.7, h=3.35);
        }

        // Screw clearance holes through the roof and bosses, plus shallow countersinks on top.
        for (sx = [-1, 1], sy = [-1, 1]) {
            x = sx*(case_x/2-mount_inset_x);
            y = sy*(case_y/2-mount_inset_y);
            translate([x, y, -0.1]) cylinder(d=screw_d, h=case_z+0.2);
            translate([x, y, case_z-countersink_depth])
                cylinder(d1=screw_d, d2=countersink_d, h=countersink_depth+0.1);
        }
    }
}

module display_retainer() {
    difference() {
        translate([0, display_center_y, case_z-roof-3.2])
            rounded_prism(retainer_outer_x, retainer_outer_y, retainer_thickness, 2);
        
            
        for (sx = [-1, 1], sy = [-1, 1]) {
            translate([sx*retainer_screw_x, display_center_y+sy*retainer_screw_y, case_z-roof-3.3])
                cylinder(d=retainer_screw_d, h=retainer_thickness+0.2);
        }
        // Clearance notch so the centered button does not collide with the frame rim.
        translate([button_center_x, button_center_y, case_z-roof-3.3])
            cylinder(d=button_hole_d+0.4, h=retainer_thickness+0.2);
    }
}

module label() {
    translate([0, label_center_y, case_z])
        linear_extrude(height=label_height)
            text(label_text, size=label_size, font="Liberation Sans:style=Bold", halign="center", valign="center", spacing=1.05);
}

module case_with_label() {
    union() {
        enclosure();
        label();
    }
}

// Selectable export parts: -D 'part="case"', "retainer", or "all".
part = "all";
if (part == "case" || part == "all") color("#f9d72c") case_with_label();
if (part == "retainer" || part == "all") color("#f9d72c") display_retainer();
