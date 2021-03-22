
void MPU6886Test()
{
    float accX = 0;
    float accY = 0;
    float accZ = 0;

    double theta, last_theta = 0;
    double phi, last_phi = 0;
    double alpha = 0.2;

    line_3d_t x = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 0}};
    line_3d_t y = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 0}};
    line_3d_t z = {
        .start_point = {0, 0, 0},
        .end_point = {0, 0, 30}};

    line_3d_t rect_source[12];
    line_3d_t rect_dis;
    for (int n = 0; n < 12; n++)
    {
        rect_source[n].start_point.x = rect[n].start_point.x * 15;
        rect_source[n].start_point.y = rect[n].start_point.y * 15;
        rect_source[n].start_point.z = rect[n].start_point.z * 15;
        rect_source[n].end_point.x = rect[n].end_point.x * 15;
        rect_source[n].end_point.y = rect[n].end_point.y * 15;
        rect_source[n].end_point.z = rect[n].end_point.z * 15;
    }

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {

		M5.Imu.getAccelData(&accX, &accY, &accZ);
        //M5.MPU6886.getAccelData(&accX, &accY, &accZ);
        if ((accX < 1) && (accX > -1))
        {
            theta = asin(-accX) * 57.295;
        }
        if (accZ != 0)
        {
            phi = atan(accY / accZ) * 57.295;
        }

        theta = alpha * theta + (1 - alpha) * last_theta;
        phi = alpha * phi + (1 - alpha) * last_phi;

        Disbuff.fillRect(0, 0, 160, 80, BLACK);
        Disbuff.setTextSize(1);
        Disbuff.setCursor(10, 55);
        Disbuff.printf("%.2f", theta);
        Disbuff.setCursor(10, 65);
        Disbuff.printf("%.2f", phi);
        //Displaybuff();
        delay(20);

        z.end_point.x = 0;
        z.end_point.y = 0;
        z.end_point.z = 30;
        RotatePoint(&z.end_point, theta, phi, 0);
        RotatePoint(&z.end_point, &x.end_point, -90, 0, 0);
        RotatePoint(&z.end_point, &y.end_point, 0, 90, 0);

        for (int n = 0; n < 12; n++)
        {
            RotatePoint(&rect_source[n].start_point, &rect_dis.start_point, theta, phi, (double)0);
            RotatePoint(&rect_source[n].end_point, &rect_dis.end_point, theta, phi, (double)0);
            printLine3D(&Disbuff, &rect_dis, WHITE);
        }
        //Disbuff.fillRect(0,0,160,80,BLACK);
        printLine3D(&Disbuff, &x, RED);
        printLine3D(&Disbuff, &y, GREEN);
        printLine3D(&Disbuff, &z, BLUE);
        //Disbuff
        Disbuff.setTextColor(WHITE);
        Disbuff.setTextSize(1);
        Disbuff.fillRect(0,0,52,18,Disbuff.color565(20,20,20));
        Disbuff.drawString("MPU6886",5,5,1);
        Displaybuff();

        last_theta = theta;
        last_phi = phi;

        M5.update();
		checkAXPPress();
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
		checkAXPPress();
        delay(10);
    }
}


