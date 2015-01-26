//
//  Shader.fsh
//  ios
//
//  Created by Saint Hsu on 23/1/15.
//  Copyright (c) 2015 Saint Hsu. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
