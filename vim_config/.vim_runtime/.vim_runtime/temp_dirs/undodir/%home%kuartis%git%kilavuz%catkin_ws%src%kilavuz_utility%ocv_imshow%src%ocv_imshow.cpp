Vim�UnDo� L(���G	���xZf�U��W�}�qh|�   .   %     ros::param::get("topic", param);                              ^�XN    _�                            ����                                                                                                                                                                                                                                                                                                                                                             ^�W     �         &      *  ros::init(argc, argv, "image_listener");5�_�                           ����                                                                                                                                                                                                                                                                                                                                                             ^�WZ     �      !   '        �         &    5�_�                            ����                                                                                                                                                                                                                                                                                                                                                             ^�W]     �      !   *         5 }5�_�                           ����                                                                                                                                                                                                                                                                                                                                                             ^�W`     �          *      
   4   ...5�_�                           ����                                                                                                                                                                                                                                                                                                                                                             ^�Wb     �         *         3 {5�_�                           ����                                                                                                                                                                                                                                                                                                                                                             ^�W     �         *        ros::NodeHandle nh;5�_�                           ����                                                                                                                                                                                                                                                                                                                                                             ^�W�     �         +        �         *    5�_�      	                     ����                                                                                                                                                                                                                                                                                                                                                             ^�W�     �         +        std::string param;5�_�      
           	          ����                                                                                                                                                                                                                                                                                                                                                             ^�W�     �         +        std::string paciram;5�_�   	              
           ����                                                                                                                                                                                                                                                                                                                                                  v       ^�W�     �      !   +           ...5�_�   
                         ����                                                                                                                                                                                                                                                                                                                                                  v       ^�W�     �      !   +      -     ros::param::get("~private_name", param);5�_�                           ����                                                                                                                                                                                                                                                                                                                                                  v       ^�W�     �         +        if (nh.hasParam("my_param"))5�_�                           ����                                                                                                                                                                                                                                                                                                                                                  v       ^�W�     �         +        if (nh.hasParam("topic"))     {�         +        if (nh.hasParam("topic"))5�_�                             ����                                                                                                                                                                                                                                                                                                                                                v       ^�W�     �       "   +        �       "   *    5�_�                    !       ����                                                                                                                                                                                                                                                                                                                                                v       ^�W�     �       #   +        else{}5�_�                    !       ����                                                                                                                                                                                                                                                                                                                                                v       ^�W�     �   !   #   -            �   !   #   ,    5�_�                    "       ����                                                                                                                                                                                                                                                                                                                                                v       ^�W�     �   !   #   -            topi = ""5�_�                    "       ����                                                                                                                                                                                                                                                                                                                                                v       ^�W�     �   !   #   -            topi = "camera_side"5�_�                    "   
    ����                                                                                                                                                                                                                                                                                                                                                v       ^�W�     �   !   #   -            topi = "camera_side";5�_�                    '   2    ����                                                                                                                                                                                                                                                                                                                                                v       ^�W�     �   &   (   -      R  image_transport::Subscriber sub = it.subscribe("camera_side", 1, imageCallback);5�_�                    '   1    ����                                                                                                                                                                                                                                                                                                                                                v       ^�W�    �   &   (   -      G  image_transport::Subscriber sub = it.subscribe("", 1, imageCallback);5�_�                           ����                                                                                                                                                                                                                                                                                                                                                v       ^�W�     �         -    �         -    5�_�                           ����                                                                                                                                                                                                                                                                                                                                                  v       ^�W�    �         .      #include <ros/ros.h>5�_�                             ����                                                                                                                                                                                                                                                                                                                                           "       v   "    ^�XM    �      !   .      %     ros::param::get("topic", param);5��