#!/usr/bin/python3

# This script requires engine changes for use.

# Remove the mario animation converter or atleast deactivate it, this script does the same thing.

# Todo:
# Lateral translation and no vertical translation support
# Finish animation system changes
# Bombomb geo not being merged proprely

import sys
import os
import math
import threading
import random

def clean(path):
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith('.old'):
                old_file_path = os.path.join(root, file)
                new_file_path = os.path.join(root, file[:-4])
                if os.path.exists(new_file_path):
                    os.remove(new_file_path)
                os.rename(old_file_path, new_file_path)
                
def print_info(string: str):
    sys.stderr.write(f"{string}\n")

def string_to_value(string: str, isS16=False):
    try:
        if string[:2] == "0x":
            hexValue = string.replace("0x", "")
            uintValue = int(hexValue, 16)
            intValue = uintValue
            if isS16:
                if (uintValue & 0x8000):
                    # Handle negative s16 value
                    intValue = -(uintValue - 0x8000)
                
            return intValue
    except:
        pass
    
    try:
        return int(string)
    except:
        try:
            return float(string)
        except:
            return string

def clamp(num, minValue, maxValue):
   return max(min(num, maxValue), minValue)

def degrees(value):
    return (value / 32767.0) * 180.0

def radians(value):
    return math.radians(degrees(value))

def quaternion_from_vec3s(x, y, z):
    psi = radians(x)
    theta = radians(y)
    phi = radians(z)

    """
    Convert Euler angles in XYZ order to quaternion
    
    Args:
        phi: rotation around x-axis, in radians
        theta: rotation around y-axis, in radians
        psi: rotation around z-axis, in radians
    
    Returns:
        Quaternion as a list [w, x, y, z]
    """
    c1 = math.cos(phi/2)
    s1 = math.sin(phi/2)
    c2 = math.cos(theta/2)
    s2 = math.sin(theta/2)
    c3 = math.cos(psi/2)
    s3 = math.sin(psi/2)
    
    w = c1*c2*c3 - s1*s2*s3
    x = s1*c2*c3 + c1*s2*s3
    y = c1*s2*c3 - s1*c2*s3
    z = c1*c2*s3 + s1*s2*c3
    
    return [w, x, y, z]

def s16(value):
    return clamp(int(value * 32768.0), -32768, 32767)

def s16_quaternion(quaternion):
    return [s16(quaternion[0]), s16(quaternion[1]), s16(quaternion[2]), s16(quaternion[3])]


def quaternion_mul(q1, q2):
    w1, x1, y1, z1 = q1
    w2, x2, y2, z2 = q2
    
    q1[3] = w1*w2 - x1*x2 - y1*y2 - z1*z2
    q1[0] = w1*x2 + x1*w2 + y1*z2 - z1*y2
    q1[1] = w1*y2 + y1*w2 + z1*x2 - x1*z2
    q1[2] = w1*z2 + z1*w2 + x1*y2 - y1*x2

    return q1

class BoneElement:
    maxFrames: int = 0
    offset: int = 0
    frames = []

class Define:
    name = ""
    values = []
    macros = ""
    
    def __init__(self, name, values, macros):
        self.name = name
        self.values = values
        self.macros = macros

class ArrayOrStruct:
    keywords = []
    type = ""    
    name = ""
    values = []
    ogString: str
    file: str
    isArray = False
    
    def __init__(self, keywords, type, name, values, ogString, isArray, file):
        self.keywords = keywords
        self.type = type
        self.name = name
        self.values = values
        self.ogString = ogString
        self.isArray = isArray
        self.file = file

class GeoLayoutCommand:
    command:str = ""
    values = []
    parent = None
    children = []
    beforeChildMacros = ""
    afterChildMacros = ""
    macros = ""
    merge = False
    remove = False
    firstCommand = None
    isAnimatable = False
    needsReexport = False
    isReturn = False
    boneIndexes = []
    boneAmount = 0
    geolayoutArray = None
    actor = None
    childIndex = 0
    cExport = ""

    def __init__(self, command, values, parent, children, macros, firstCommand, geolayoutArray, actor):
        self.command = command
        self.values = values
        self.parent = parent
        self.children = children
        self.firstCommand = firstCommand
        self.boneIndexes = []
        self.geolayoutArray = geolayoutArray
        self.actor = actor
        self.macros = macros
    
    def read_geo_array(self, geolayoutArray):
        firstDefine = geolayoutArray.values[0]
        self.command = firstDefine.name
        self.values = firstDefine.values
        self.parent = None
        self.macros = firstDefine.macros
        self.firstCommand = self
        self.geolayoutArray = geolayoutArray
        parentGeoCommand = self
        
        for define in geolayoutArray.values[1:]:
            if define.name == "GEO_OPEN_NODE":
                if len(parentGeoCommand.children) > 0:
                    parentGeoCommand = parentGeoCommand.children[-1]
                    parentGeoCommand.beforeChildMacros = define.macros
            elif define.name == "GEO_CLOSE_NODE":
                if parentGeoCommand.parent != None:
                    parentGeoCommand.afterChildMacros = define.macros
                    parentGeoCommand = parentGeoCommand.parent
            elif define.name == "GEO_RETURN":
                self.isReturn = True
                continue
            elif define.name == "GEO_END":
                continue
            else:
                if define.name == "GEO_ANIMATED_PART":
                    self.isAnimatable = True

                child = GeoLayoutCommand(define.name, [], parentGeoCommand, [], define.macros, self, self.geolayoutArray, self.actor)
                for value in define.values:
                    child.values.append(value)

                parentGeoCommand.children.append(child)
    
    def find_bone_indexes(self):
        if self.command == "GEO_ANIMATED_PART":
            self.firstCommand.boneIndexes.append(self)
            self.boneAmount += 1
        if self.command == "GEO_SWITCH_CASE" and self.children:
            self.children[0].find_bone_indexes()
        else:
            for child in self.children:
                child.find_bone_indexes()

    def find_unused_bones(self):
        shouldMerge = False
        shouldRemove = False
        
        if self.parent:
            # If an animation bone is null it might potentially be uncessary
            if self.command == "GEO_ANIMATED_PART" and self.values[4] == "NULL" and self.firstCommand.boneIndexes[0] != self:
                # To make sure the bone should be merged, check every child and test if all of them are animation bones and dont have any translation.
                for child in self.children:
                    if child.command == "GEO_ANIMATED_PART" and (child.values[1:4] == [0, 0, 0]):
                        shouldMerge = True
                    else:
                        shouldMerge = False
                        break
                
            # If an animation bone has only one child, and that child is null and has no translation the child should be merged
            if self.parent.command == "GEO_ANIMATED_PART" and len(self.parent.children) == 1 and self.values[1:4] == [0, 0, 0] and self.values[4] == "NULL":
                shouldMerge = True

            if self.command == "GEO_ANIMATED_PART" and len(self.children) == 0 and self.values[4] == "NULL":
                shouldRemove = True

        self.merge = shouldMerge
        self.remove = shouldRemove

        if self.merge or self.remove:
            self.firstCommand.needsReexport = True

        for child in self.children:
            child.find_unused_bones()
    
    def fix_geolayout(self):
        for child in self.children.copy():
            child.fix_geolayout()  

        if self.remove and self in self.parent.children:
            index = self.parent.children.index(self)
            self.parent.children.pop(index)

        # The current bone should be replaced by its children, each child bone should get the current bone´s translation
        if self.merge:
            for child in self.children:
                if child.command == "GEO_ANIMATED_PART":
                    child.values[1] += self.values[1]
                    child.values[2] += self.values[2]
                    child.values[3] += self.values[3]
                
                child.parent = self.parent
                self.parent.children.append(child)

            if self in self.parent.children:
                index = self.parent.children.index(self)
                self.parent.children.pop(index)
    
    def convert_geo_to_c(self):
        self.cExport = f"const GeoLayout {self.geolayoutArray.name}[] = {{\n{self.to_string()[0]}}};"
        #print_info(self.cExport)

    def reexport(self):
        self.actor.geolayoutTexts[self.geolayoutArray.file] = self.actor.geolayoutTexts[self.geolayoutArray.file].replace(self.geolayoutArray.ogString, self.cExport)
        rewrite_files(self.actor.geolayoutTexts, self.geolayoutArray.file)   

    def change_macro_state(self, string, macros, newMacros):
        if macros != newMacros:
            if len(newMacros) > 2:
                string += f"#{newMacros}\n"
            return string, newMacros
        return string, macros
    
    def to_string(self, string="", macros="", level=1):
        tabs = "\t" * level

        string, macros = self.change_macro_state(string, macros, self.macros)

        values = ""
        if self.values:
            values = ', '.join([str(elem) for elem in self.values])

        string += f"{tabs}{self.command}({values}),\n"

        if self.children:
            string, macros = self.change_macro_state(string, macros, self.beforeChildMacros)
            string += f"{tabs}GEO_OPEN_NODE(),\n"

        for child in self.children:
            result = child.to_string(string, macros, level + 1)
            string = result[0]
            macros = result[1]

        if self.children:
            string, macros = self.change_macro_state(string, macros, self.afterChildMacros)
            string += f"{tabs}GEO_CLOSE_NODE(),\n"
            
            if level == 1:
                if self.isReturn:
                    string += f"{tabs}GEO_RETURN(),\n"
                else:
                    string += f"{tabs}GEO_END(),\n"    

        return string, macros
        
def find_all_arrays_in_text(string, file, arrays):
    #print_info(f"Searching for arrays in file {file}.")
    
    charIndex = 0
    end = len(string)
    readingKeywords = True
    keywordsStart = 0
    keywordStart = 0
    valueStart = 0
    isArray = False
    readingValues = False
    inComment = False
    multiLineCommment = False
    previousChar = ""
    inParen = 0

    readingMacro = False
    readMacroName = False
    readMacro = False
    macroStart = 0
    currentMacro = ""

    isDefine = False
    defineStart = 0

    define:Define = Define("", [], "")

    keywords = []
    values = []

    # Begin parsing
    while charIndex < end:
        char = string[charIndex]

        # Handle comments
        if previousChar == "/" and char == "/":
            inComment = True
        
        if previousChar == "/" and char == "*":
            inComment = True
            multiLineCommment = True

        if inComment:
            if char == "\n" and not multiLineCommment:
                inComment = False
            
            if multiLineCommment and previousChar == "*" and char == "/":
                inComment = False
                multiLineCommment = False 
        
        # If not in comment continue parsing
        if not inComment:
            if readingMacro:
                if char == "&" or char == "|":
                    readMacro = True

                if (previousChar == " " or char == "\n" or char == "\t") and (char != "&" and char != "|"):
                    if readMacroName:
                        macroName = string[macroStart:charIndex].strip()
                        if macroName == "ifdef" or macroName == "ifndef":
                            readMacro = True
                            readMacroName = False

                    if not readMacro:
                        currentMacro = string[macroStart:charIndex].strip()    
                        #print_info(currentMacro)                    
                        if macroStart > keywordStart < charIndex:
                            keywordStart = charIndex
                        
                        if macroStart > valueStart < charIndex:
                            valueStart = charIndex
                        
                        if macroStart > defineStart < charIndex:
                            defineStart = charIndex
                        readingMacro = False
                    readMacro = False

            if not readingMacro:
                if char == "#":
                    macroStart = charIndex + 1
                    readMacroName = True
                    readingMacro = True
            
            if not readingMacro:
                # Reading keywords step
                if readingKeywords:
                    if char == "\n" or char == ";":
                        keywordsStart = charIndex + 1
                        keywordStart = keywordsStart
                        currentMacro = ""
                    
                    if char == "[" or char == "=" or char == " ":
                        keyword = string[keywordStart:charIndex]
                        if keyword != " " and keyword != "":
                            keywords.append(string_to_value(string[keywordStart:charIndex].strip()))

                        keywordStart = charIndex + 1
                        
                        if char != " ":
                            if char == "[":
                                isArray = True

                            readingKeywords = False

                # Reading values step ( data after "={" )
                if readingValues:
                    if isDefine and inParen == True and (char == ")" or char == ",") and string[valueStart:charIndex] != "":
                        define.values.append(string_to_value(string[valueStart:charIndex].strip()))

                    if char == '}':
                        structData = ArrayOrStruct(keywords[:-2], keywords[-2], keywords[-1], values, string[keywordsStart:charIndex + 2], isArray, file)
                        arrays[structData.name] = structData
                        readingKeywords = True
                        isArray = False
                        readingValues = False
                        previousChar = ""
                        inParen = 0
                        keywords = []
                        values = []
                    elif char == '(':
                        if inParen == False:
                            defineStart = charIndex + 1
                            define = Define(string_to_value(string[valueStart:charIndex].strip()), [], currentMacro)
                            valueStart = defineStart
                            isDefine = True
                        inParen = True
                    elif char == ')':
                        inParen = False
                    elif char == ',': 
                        if inParen == False:
                            if isDefine == True:
                                values.append(define)
                                isDefine = False
                            else:
                                isS16 = len(keywords) >= 2 and keywords[-2] == "s16"
                                values.append(string_to_value(string[valueStart:charIndex].strip(), isS16))
                        valueStart = charIndex + 1 
                
                # In between stage, = or [] has been reached so we are no longer reading keywords but we are still not reading values until the first {
                else:
                    if char == "{":
                        readingValues = True
                        valueStart = charIndex + 1
                
        previousChar = char
        charIndex += 1

def read_arrays_in_file(path, filesText, arrays):
    with open(path, "r") as f:
        fileText = f.read()
        filesText[path] = fileText
        find_all_arrays_in_text(fileText, path, arrays)

class OldAnimationData:
    elements = []

    maxFrame = 0
    boneCount: int = 0
    boneElementCount: int = 0

    anim = None
    actor = None

    valuesArray:ArrayOrStruct
    indexesArray:ArrayOrStruct

    def __init__(self, anim, valuesArray, indexesArray, actor):
        self.anim = anim
        self.valuesArray = valuesArray
        self.indexesArray = indexesArray
        self.actor = actor

    def fetch_animation_indexes(self):
        indexes = self.indexesArray.values

        self.boneElementCount = int(len(indexes) / 2)
        self.boneCount = clamp(self.boneElementCount // 3 - 1, 0, math.inf)

        print_info(f"Reading animation indexes from {self.indexesArray.name}. Bone elements: {self.boneElementCount}. Bones: {self.boneCount }")

        self.elements.clear()
        
        i = 0
        while(i < self.boneElementCount * 2):
            boneElement = BoneElement()
            
            boneElement.maxFrames = indexes[i]
            boneElement.offset = indexes[i + 1]

            if(boneElement.maxFrames > self.maxFrame):
                self.maxFrame = boneElement.maxFrames

            self.elements.append(boneElement)

            i += 2
        
    def fetch_animation_values(self):
        print_info(f"Reading animation values from {self.valuesArray.name}. Frames: {self.maxFrame}")

        values = self.valuesArray.values

        for i in range(self.boneElementCount):
            boneElement = self.elements[i]

            for j in range(boneElement.maxFrames):
                pos = boneElement.offset + j
                x = values[pos]

                boneElement.frames.append(x)

def is_bit_true(value, bitIndex):
    return ((value >> bitIndex) & 1) == 1

class OldAnimation:
    actor = None
    name: str = ""

    startFrame: int = 0
    loopStart: int = 0
    loopEnd: int = 0
    animYTransDivisor: int = 0
    flags = []

    animData = None

    valuesArray = None
    indexesArray = None

    structArray = None

    def __init__(self, actor):
        self.actor = actor
        self.elements = []

    def read_animation_array(self, array, animFilesArrays):
        print_info(f"Reading animation {self.name}´s struct.")
        self.structArray = array
        self.name = array.name
        flagsAsValue = array.values[0]

        self.flags = ["0"]
        if is_bit_true(flagsAsValue, 0):
            self.flags.append("ANIM_FLAG_NOLOOP")
        if is_bit_true(flagsAsValue, 1):
            self.flags.append("ANIM_FLAG_FORWARD")
        if is_bit_true(flagsAsValue, 2):
            self.flags.append("ANIM_FLAG_NO_ACCEL")
        if is_bit_true(flagsAsValue, 3):
            self.flags.append("ANIM_FLAG_HOR_TRANS")
        if is_bit_true(flagsAsValue, 4):
            self.flags.append("ANIM_FLAG_VERT_TRANS")
        if is_bit_true(flagsAsValue, 5):
            self.flags.append("ANIM_FLAG_DISABLED")
        if is_bit_true(flagsAsValue, 6):
            self.flags.append("ANIM_FLAG_NO_TRANS")
        if is_bit_true(flagsAsValue, 7):
            self.flags.append("ANIM_FLAG_UNUSED")

        self.animYTransDivisor = array.values[1]
        self.startFrame = array.values[2]
        self.loopStart = array.values[3]
        self.loopEnd = array.values[4]
        self.valuesArray = animFilesArrays[array.values[6]]
        self.indexesArray = animFilesArrays[array.values[7]]
        self.length = array.values[8]

    def fetch_animation_data(self):
        if self.indexesArray.name not in self.actor.oldAnimData:
            animData = OldAnimationData(self, self.valuesArray, self.indexesArray, self.actor)
            animData.fetch_animation_indexes()
            animData.fetch_animation_values()
            self.actor.oldAnimData[self.indexesArray.name] = animData

        self.animData = self.actor.oldAnimData[self.indexesArray.name]
        
def rewrite_files(animFilesText, *files):
    for filePath in files:
        newFilePath = filePath
        print_info(filePath)
        if filePath[-4:] != ".old":
            oldFilePath = f"{filePath}.old"
            if not os.path.exists(oldFilePath):
                os.rename(filePath, oldFilePath)
            print_info(f"File {newFilePath} being rewritten.")
            file = open(newFilePath,"w")
            file.write(animFilesText[newFilePath])
            file.close()

class NewAnimationData:
    oldAnimData = None
    actor = None
    anim = None
    
    name = ""

    boneCount = 0
    maxFrame = 0

    frames = []

    valuesArray:ArrayOrStruct
    indexesArray:ArrayOrStruct

    cFrames: str = ""
    framesSize = 0
    geolayout:GeoLayoutCommand = None

    def __init__(self):
        self.frames = []

    def from_old_format(self, oldAnimData):
        self.oldAnimData = oldAnimData
        self.actor = self.oldAnimData.actor
        self.anim = self.oldAnimData.anim

        self.valuesArray = self.oldAnimData.valuesArray
        self.indexesArray = self.oldAnimData.indexesArray

        self.name = f"{self.valuesArray.name}_frames"
        self.maxFrame = self.oldAnimData.maxFrame

        print_info(f"Converting animation data of {oldAnimData.valuesArray.name} to new format. Max frames: {oldAnimData.maxFrame}. Element amount: {len(oldAnimData.elements)}. Flags: {self.oldAnimData.anim.flags}")

        for geolayout in self.actor.geolayouts:
            self.geolayout = geolayout
            if len(geolayout.boneIndexes) == self.boneCount:
                break

        realBoneCount = len(self.geolayout.boneIndexes)

        frame = 0
        self.frames.clear()
        values = []
        vec4ss = []
        while frame < self.maxFrame:
            j = 0 
            applyRoot = True
            applyRotations = False
            merge = [0,0,0,0]
            boneIndex = 0
            while j < self.oldAnimData.boneElementCount:
                if boneIndex >= realBoneCount:
                    bone = GeoLayoutCommand("", [], None, [], "", None, None, None)
                else:
                    bone = self.geolayout.boneIndexes[boneIndex]

                boneElement = self.oldAnimData.elements[j]

                if(frame >= boneElement.maxFrames):
                    values.append(boneElement.frames[boneElement.maxFrames-1])
                else:
                    values.append(boneElement.frames[frame])
                
                valueCount = len(values)
            
                if applyRoot:
                    if valueCount == 2 and "ANIM_FLAG_HOR_TRANS" in self.anim.flags:
                        vec4ss.append([values[0], 0, values[1], 1])
                        values.clear()
                        applyRoot = False
                    elif valueCount == 1 and "ANIM_FLAG_VERT_TRANS" in self.anim.flags:
                        vec4ss.append([0, values[0], 0, 1])
                        values.clear()
                        applyRoot = False
                    elif "ANIM_FLAG_NO_TRANS" in self.anim.flags:
                        vec4ss.append([0, 0, 0, 1])
                        applyRoot = False
                
                if valueCount == 3:
                    if not bone.remove:         
                        quaternion = quaternion_from_vec3s(values[0], values[1], values[2])

                        if bone.merge:
                            if bone.parent and bone.parent.merge:
                                merge = quaternion_mul(quaternion, merge)
                            else:
                                merge = quaternion
                        else:
                            if bone.parent and bone.parent.merge:
                                quaternion = quaternion_mul(quaternion, merge)
                            
                            vec4ss.append(s16_quaternion(quaternion))
                    boneIndex += 1
                    values.clear()

                j += 1
            
            self.frames.append(vec4ss)
            vec4ss.clear()

            frame += 1
        
        self.boneCount = len(self.frames[0])
        self.framesSize = (self.boneCount) * self.maxFrame

    def generate_frame_data_to_c(self, smallFrameData=False):
        framesString = "{\n"
        for frame in self.frames:
            for vec4s in frame:
                framesString += f"\t{{{vec4s[0]}, {vec4s[1]}, {vec4s[2]}, {vec4s[3]}}},\n"
            framesString += "\n\n"
        framesString += "}"
        return framesString
    
    def convert_frame_data_to_c(self):
        self.cFrames = f"static const Vec4s {self.name}[{self.framesSize}] = {self.generate_frame_data_to_c()};"        
    
    def reexport(self):
        self.actor.animFilesTexts[self.valuesArray.file] = self.actor.animFilesTexts[self.valuesArray.file].replace(self.valuesArray.ogString, self.cFrames)
        self.actor.animFilesTexts[self.indexesArray.file] = self.actor.animFilesTexts[self.indexesArray.file].replace(self.indexesArray.ogString, "")
        rewrite_files(self.actor.animFilesTexts, self.valuesArray.file, self.indexesArray.file)   

class NewAnimation:
    oldAnim: OldAnimation
    name: str = ""
    
    frameArrayName: str = ""
    startFrame: int = 0
    loopStart: int = 0
    loopEnd: int = 0
    animYTransDivisor: int = 0
    flags: int = 0
    
    animData = None

    cStructString: str = ""
    cStruct: str = ""

    def __init__(self):
        self.frames = []

    def from_old_format(self, oldAnim):
        print_info(f"Converting animation {oldAnim.name} to new format.")
        self.oldAnim = oldAnim
        self.name = oldAnim.name
        
        self.frameArrayName = f"{oldAnim.valuesArray.name}_frames"
        self.startFrame = oldAnim.startFrame
        self.loopStart = oldAnim.loopStart
        self.animYTransDivisor = oldAnim.animYTransDivisor
        self.flags = oldAnim.flags

        toRemove = ["ANIM_FLAG_HOR_TRANS", "ANIM_FLAG_VERT_TRANS", "ANIM_FLAG_NO_TRANS"]
        self.flags = [x for x in self.flags if x not in toRemove]

        self.actor = self.oldAnim.actor

        self.animData = self.actor.newAnimData[oldAnim.indexesArray.name]

        print_info(f"Converted animation frame array has {len(self.frames)} frames.")

    def generate_struct_data_to_c(self, isDma=False):
        frameArrayName = self.frameArrayName
        if isDma:
            frameArrayName = f"{{(const Vec4s*)(offsetof(struct MarioAnimsObj, {frameArrayName}) - offsetof(struct MarioAnimsObj, {self.name}))}}"
        flags = " | ".join(self.flags)
        return f"{{\n/*Frame array*/ {frameArrayName},\n/*Start frame*/ {str(self.startFrame)},\n/*Loop start*/ {str(self.loopStart)},\n/*Max Frame*/ {str(self.animData.maxFrame)},\n/*Animation Y trans divisor*/{str(self.animYTransDivisor)},\n/*Bone Count*/ {str(self.animData.boneCount)},\n/*Flags*/ ({flags}),\n}}"
    
    def convert_anim_to_c(self):
        print_info(f"Converting animation {self.name} to c.")
        
        self.cStructString = f"static const struct Animation {self.name}"        

        if self.oldAnim.structArray.isArray:
            self.cStructString += "[]"

        self.cStructString += f" =\n{self.generate_struct_data_to_c()};"
        
        print_info(f"Animation {self.name} has been converted to c.")

    def reexport(self):
        self.actor.animFilesTexts[self.oldAnim.structArray.file] = self.actor.animFilesTexts[self.oldAnim.structArray.file].replace(self.oldAnim.structArray.ogString, self.cStructString)

        rewrite_files(self.actor.animFilesTexts, self.oldAnim.structArray.file, self.oldAnim.valuesArray.file, self.oldAnim.indexesArray.file)   

class Actor:
    animFolder = ""
    geolayoutPath = ""

    animFilesArrays = dict()
    animFilesTexts = dict()

    oldAnimData = dict()
    newAnimData = dict()

    oldAnims = []
    newAnims = []

    geolayoutArrays = dict()
    geolayoutTexts = dict()
    geolayouts = []

    isMario = False

    def reexport_mario_animations(self):
        print_info("Mario animations being export as dma animation.")
        
        animAmount = len(self.newAnims)

        directory = "build/us_n64/assets/"
        if not os.path.exists(directory):
            os.makedirs(directory)

        filePath = f"{directory}/mario_anim_data.c"

        includes =  "#include \"game/memory.h\"\n#include <stddef.h>"
        
        animReferences = []
        for animIndex in range(len(self.newAnims)):
            anim = self.newAnims[animIndex]
            animReferences.append(f"struct Animation {anim.name}")
            if animIndex + 1 >= len(self.newAnims) or self.newAnims[animIndex + 1].animData.name != anim.animData.name:
                animReferences.append(f"Vec4s {str(anim.animData.name)}[{anim.animData.framesSize}]")

        animReferences = ";\n\t".join(animReferences)
        
        marioAnimsObj = f"const struct MarioAnimsObj {{\n\tu32 numEntries;\n\tconst struct Animation *addrPlaceholder;\n\tstruct OffsetSizePair entries[{animAmount}];\n{animReferences}}}"
        
        sizesAndOffsets = ""
        for anim in self.newAnims:
            offset = f"offsetof(struct MarioAnimsObj, {anim.name})"
            size = f"offsetof(struct MarioAnimsObj, {anim.name}) + sizeof(gMarioAnims.{anim.animData.name}) - offsetof(struct MarioAnimsObj, {anim.name})"
            sizesAndOffsets += f"\t{{{offset}, {size}}},\n"   

        #//{anim.animData.name}[{anim.animData.framesSize}]\n{anim.animData.cFrames}\n

        animationData = []
        for animIndex in range(len(self.newAnims)):
            anim = self.newAnims[animIndex]
            animationData.append(anim.generate_struct_data_to_c(True))
            if animIndex + 1 >= len(self.newAnims) or self.newAnims[animIndex + 1].animData.name != anim.animData.name:
                animationData.append(anim.animData.generate_frame_data_to_c())

        animationData = ",\n".join(animationData)
        
        marioAnims = f"gMarioAnims = \n{{\n\t{animAmount},\n\tNULL,\n{{{sizesAndOffsets}}},\n{animationData}\n}};"

        result = f"{includes}\n{marioAnimsObj}\n{marioAnims}"

        file = open(filePath,"w")
        file.write(result)
        file.close()
    
    def reexport_normal_animations(self):
        for anim in self.newAnims:
            anim.convert_anim_to_c()
            anim.reexport()

        for animData in self.newAnimData.values():
            animData.convert_frame_data_to_c()
            animData.reexport()

    def reexport_animations(self):
        if self.isMario:
            self.reexport_mario_animations()
        else:
            self.reexport_normal_animations()

    def fix_animation_data(self):
        for oldAnimDataKey in self.oldAnimData.keys():
            oldAnimData = self.oldAnimData[oldAnimDataKey]
            newAnimData = NewAnimationData()
            newAnimData.from_old_format(oldAnimData)
            self.newAnimData[oldAnimDataKey] = newAnimData

    def fix_animations(self):
        for anim in self.oldAnims:
            newAnim = NewAnimation()
            newAnim.from_old_format(anim)
            self.newAnims.append(newAnim)

    def fix_geos(self):
        for geolayout in self.geolayouts:
            if not geolayout.needsReexport:
                continue
            geolayout.fix_geolayout()
            geolayout.convert_geo_to_c()
            geolayout.reexport()

    def read_geolayouts(self, geoPath):
        read_arrays_in_file(geoPath, self.geolayoutTexts, self.geolayoutArrays)

        for geolayoutArray in self.geolayoutArrays.values():
            geolayout = GeoLayoutCommand("", [], None, [], "", None, geolayoutArray, self)
            geolayout.read_geo_array(geolayoutArray)
            if not geolayout.isAnimatable:
                continue

            geolayout.find_bone_indexes()
            #geolayout.find_unused_bones()

            if geolayout.needsReexport != True and not self.oldAnims:
                continue

            self.geolayouts.append(geolayout)

    def read_animation_data(self):
        for array in self.animFilesArrays.values():
            if array.type == "Animation":
                if len(array.values) == 7:
                    return
                print_info(f"Processing animation {array.name}")
                anim = OldAnimation(self)
                anim.read_animation_array(array, self.animFilesArrays)
                anim.fetch_animation_data()

                self.oldAnims.append(anim)

    def search_through_animations(self):
        fileNames = os.listdir(self.animFolder)
        fileNames.sort()
        for name in fileNames:
            if name[-4:] == ".old":
                continue
            animPath = os.path.join(self.animFolder, name)
            read_arrays_in_file(animPath, self.animFilesTexts, self.animFilesArrays)

    def process_animations(self):
        self.search_through_animations()
        self.read_animation_data()

    def read_data(self):
        if os.path.isdir(self.animFolder):
            self.process_animations()

        if os.path.exists(f"{self.geolayoutPath}.old"):
            self.read_geolayouts(f"{self.geolayoutPath}.old")
        elif os.path.exists(self.geolayoutPath):
            self.read_geolayouts(self.geolayoutPath)

    def process_actor(self):
        self.read_data()
        self.fix_geos()
        self.fix_animation_data()
        self.fix_animations()
        self.reexport_animations()

    def __init__(self, animFolder, geolayoutPath, isMario):
        self.animFolder = animFolder
        self.geolayoutPath = geolayoutPath
        
        self.animFilesArrays = dict()
        self.animFilesTexts = dict()
        self.oldAnims = []
        self.newAnims = []

        self.geolayoutArrays = dict()
        self.geolayoutTexts = dict()
        self.geolayouts = []
        
        self.oldAnimData = dict()
        self.newAnimData = dict()

        self.isMario = isMario
        
def process_actor_folder(name, dir):
    animDir = f"{dir}/anims/"
    geoPath = f"{dir}/geo.inc.c"

    actor = Actor(animDir, geoPath, False)
    actor.process_actor()

def search_through_actor_folder():
    actorDir = "actors/"

    for name in os.listdir(actorDir):
        dir = os.path.join(actorDir, name)
        if os.path.isdir(dir):
            process_actor_folder(name, dir)

def search_through_level_folder():
    levelDir = "levels/"

    for root, dirs, files in os.walk(levelDir):
        for dir in dirs:
            folder = os.path.join(root, dir)
            animPath = os.path.join(folder, "anim.inc.c")
            if os.path.exists(animPath):
                geoPath = os.path.join(folder, "geo.inc.c")
                levelModel = Actor(folder, geoPath, False)
                levelModel.process_actor()

            
def process_mario():
    marioAnimDir = f"assets/anims/"
    marioGeoPath = f"actors/mario/geo.inc.c"

    mario = Actor(marioAnimDir, marioGeoPath, True)
    mario.process_actor()

def start_converter(): 
    print_info("Quaternion converter started.") 
    
    process_mario()

    search_through_actor_folder()
    search_through_level_folder()

    print_info("Quaternion converter finished.") 

langs = "No Command"
if len(sys.argv) > 1:
    langs = sys.argv[1]

if "clean" in langs:
    clean("actors/")
    clean("assets/anims/")
    clean("levels")
else:
    start_converter()
    pass

