// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

void camera_do (void) {
	cam_pos = prx - 120;
	if (cam_pos < section_x0) cam_pos = section_x0;
	if (cam_pos > section_x1) cam_pos = section_x1;
}
