#CSci-4611 Assignment 4: So You Think Ants Can Dance
This is a discription file for assignment 4 - Ants Dance, created by Junyue Tao.

###Hierarchical transform

Function DrawBoneRecursive() is used to build the hierarchical transforms. The hierarchical transformation matrices are 
defined according to the skeleton of the ant.

    Matrix4 ctm = parent_transform * 
                    skeleton_.BoneSpaceToRotAxesSpace(bone_name) * 
                    pose_.JointRotation(bone_name) * 
                    skeleton_.RotAxesSpaceToBoneSpace(bone_name);
Later drawing the corresponding part of the ant based on the current, transformed orgin location.
    
    Matrix4 child_root_transform = ctm * skeleton_.BoneSpaceToChildrenSpace(bone_name);

    for (int i=0; i<skeleton_.num_children(bone_name); i++) {
        DrawBoneRecursive(skeleton_.child_bone(bone_name, i), child_root_transform, view_matrix, proj_matrix);
    }


###Draw the ant
The QuickShape functions are being used to draw the body parts of the ant, which are represented as some geometries like
spheres, cubes, cylinders and cones. Below are examples drawing a head.

    if (bone_name == "head") {
        quick_shapes_.DrawLineSegment(ctm, view_matrix, proj_matrix,
            stick_color, origin_, origin_
            + skeleton_.BoneDirectionAndLength(bone_name), 0.01);

        quick_shapes_.DrawSphere(ctm * 
            Matrix4::RotationX(-GfxMath::PI / 10) * 
            Matrix4::Scale(Vector3(0.10, 0.15, 0.10)) * 
            Matrix4::Translation(Vector3(0, 0.2, 0.8)), view_matrix, proj_matrix, body_color);

        // draw eyes
        quick_shapes_.DrawSphere(ctm * Matrix4::Scale(Vector3(0.02, 0.02, 0.02))
            * Matrix4::Translation(Vector3(2, 5, 8.5)),
            view_matrix, proj_matrix, stick_color);
        quick_shapes_.DrawSphere(ctm * Matrix4::Scale(Vector3(0.02, 0.02, 0.02))
            * Matrix4::Translation(Vector3(-2, 5, 8.5)),
            view_matrix, proj_matrix, stick_color);

        // draw antena
        quick_shapes_.DrawCone(ctm * 
            Matrix4::Scale(Vector3(0.02, 0.02, 0.1)) * 
            Matrix4::Translation(Vector3(-2, 15, 2)) * 
            Matrix4::RotationX(GfxMath::PI / 2), view_matrix, proj_matrix, stick_color);
        quick_shapes_.DrawCone(ctm * 
            Matrix4::Scale(Vector3(0.02, 0.02, 0.1)) * 
            Matrix4::Translation(Vector3(2, 15, 2)) * 
            Matrix4::RotationX(GfxMath::PI / 2), view_matrix, proj_matrix, stick_color);

        quick_shapes_.DrawCone(ctm * 
            Matrix4::RotationX(GfxMath::PI / 13) * 
            Matrix4::Scale(Vector3(0.02, 0.1, 0.02)) * 
            Matrix4::Translation(Vector3(2, 2.6, 2)), view_matrix, proj_matrix, stick_color);
        quick_shapes_.DrawCone(ctm * 
            Matrix4::RotationX(GfxMath::PI / 13) * 
            Matrix4::Scale(Vector3(0.02, 0.1, 0.02)) * 
            Matrix4::Translation(Vector3(-2, 2.6, 2)), view_matrix, proj_matrix, stick_color);

    }


###Motions

As for additional motions, I chosed several clips from amc fles, trimmed some inappropriate frames from the front 
and back of the clips. The final loaded motions are corresponded with buttons on the upper left corner. To modify the 
ants movement, simply click the “Motion X” buttons. For the other 4 special motions, I use the same dance moves. 

They are 05_10.amc, 60_03.amc, 60_07.amc, and 60_05.amc.

    ballet_special2_.LoadFromAMC(Platform::FindFile("05_10.amc", searchPath_), *ballet_ant_.skeleton_ptr());
    ballet_special2_.TrimFront(380);
    ballet_special2_.TrimBack(200);
    ballet_special2_.CalcRelativeTranslations();

    ballet_special3_.LoadFromAMC(Platform::FindFile("60_03.amc", searchPath_), *ballet_ant_.skeleton_ptr());
    ballet_special3_.TrimFront(100);
    ballet_special3_.TrimBack(200);
    ballet_special3_.CalcRelativeTranslations();

    ballet_special4_.LoadFromAMC(Platform::FindFile("60_07.amc", searchPath_), *ballet_ant_.skeleton_ptr());
    ballet_special4_.TrimFront(100);
    ballet_special4_.TrimBack(710);
    ballet_special4_.CalcRelativeTranslations();

    ballet_special5_.LoadFromAMC(Platform::FindFile("60_05.amc", searchPath_), *ballet_ant_.skeleton_ptr());
    ballet_special5_.TrimFront(100);
    ballet_special5_.TrimBack(200);
    ballet_special5_.CalcRelativeTranslations();
