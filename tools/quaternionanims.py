#!/usr/bin/python3

# Lila’s Improved Lightweight Animations
# Todo:
# Code clean up

import struct
import sys
import os
import math
import threading
import json
from typing import List

def print_text(string: str):
    """
    Prints a string to stderr.

    Args:
        string (str): The string to print.
    """
    sys.stderr.write(f"{string}\n")

def print_error(string: str):
    """
    Prints an error message to stderr.

    Args:
        string (str): The error message to print.
    """
    print_text(f"\n\n{string}\n")

def print_info(string: str):
    """
    Prints an info message to stderr.

    Args:
        string (str): The info message to print.
    """
    print_text(string)

def print_test(string: str):
    """
    Prints a test message to stderr.

    Args:
        string (str): The test message to print.
    """
    print_text(string)

def string_to_value(string: str):
    """
    Converts a string to an integer, float, or string value.

    If the string starts with "0x", it is interpreted as a hexadecimal integer.
    Otherwise, the function tries to convert the string to an integer or float.
    If both conversions fail, the original string is returned.

    Args:
        string (str): The string to convert.

    Returns:
        int, float, or str: The converted value.
    """

    # Check if the string starts with "0x"
    if string[:2] == "0x":
        # Interpret the string as a hexadecimal integer
        hexValue = string[2:]
        intValue = int(hexValue, 16)
        return intValue

    # Try to convert the string to an integer
    try:
        return int(string)
    except:
        # Try to convert the string to a float
        try:
            return float(string)
        except:
            # If both conversions fail, return the original string
            return string

def clamp(num, minValue, maxValue):
    """
    Clamps a number to a specified range.

    Args:
        num (int or float): The number to clamp.
        minValue (int or float): The minimum value of the range.
        maxValue (int or float): The maximum value of the range.

    Returns:
        int or float: The clamped value.
    """

    return max(min(num, maxValue), minValue)


def angle(value:int) -> float:
    """
    Converts an angle from a signed 16-bit integer representation to degrees.

    The signed 16-bit integer representation uses the range of values from the minimum
    to the maximum representable value of a signed 16-bit integer to represent angles
    in the range [0, 360) degrees.

    Args:
        value (int): The signed 16-bit integer angle value.

    Returns:
        float: The angle in degrees.
    """
    return value * 360.0 / (2**16)

def is_bit_true(value, bitIndex):
    return ((value >> bitIndex) & 1) == 1

def float_to_fixed_exponent_bfloat(value: float, increment:float, exponent:int) -> int:
    """
    Converts a float to a fixed exponent "bfloat" representation.

    Args:
        value (float): The float to be converted

    Returns:
        List[float]: quaternion (wxyz)
    """

    base1Value = clamp(value + increment, 
                    increment - increment / 3.0, 
                    increment + increment / 3.0 - 0.00001)

    # Calculate the value with the new exponent value applied
    valueExpApplied = base1Value / (2.0**exponent)

    # Update the exponent value
    fixedExpValue = math.ldexp(valueExpApplied, exponent)

    # Pack the fixed exponent float into a binary format
    packed = struct.pack('f', fixedExpValue)
    # Unpack the binary format into an integer
    floatLiteral = struct.unpack('I', packed)[0]

    # Shift and mask the exponent and sign value to obtain the
    # fixed exponent bfloat representation.
    # The "bfloat" uses in total 15 bits
    bfloatValue = (floatLiteral >> 8) & 0b11111111111111111
    return bfloatValue

def fixed_exponent_bfloat_to_float(value: int, exponent: int, removal: float) -> float:
    """
    Converts a fixed exponent bfloat to a float.

    A fixed exponent bfloat is a 16-bit floating point number with a fixed exponent.
    This function converts such a number to a standard 32-bit float.

    Args:
        value (int): The value to convert.
        exponent (int): The exponent to use.
        removal (float): The value to remove from the result.

    Returns:
        float: The converted float value.
    """
    # Combine the value and exponent into a single 32-bit integer
    literal = exponent | (value << 8)

    # Unpack the 32-bit integer as a float
    f = struct.unpack('f', struct.pack('I', literal))[0]

    f -= removal

    return f

def s16_quaternion(quaternion: List[float], bfloat: bool = False) -> List[int]:
    """
    Converts a quaternion rotation to a list of signed 16-bit integers.

    A quaternion is a four-dimensional complex number system that can represent three-dimensional rotations.
    This function converts such a quaternion to a list of four signed 16-bit integers.

    Args:
        quaternion (List[float]): The quaternion to convert.
        bfloat (bool): Whether to use fixed exponent 16-bit bfloats.

    Returns:
        List[int]: The list of signed 16-bit integers representing the quaternion.
    """
    
    if bfloat:
        # Convert the quaternion to a list of fixed exponent 16-bit bfloats
        return [float_to_fixed_exponent_bfloat(x, 3.0, 128) for x in quaternion]
    else:
        # Convert the quaternion to a list of signed 16-bit integers
        return [clamp(int(x * 32766.0), -32766, 32766) for x in quaternion]

def s16_translation(translation: List[float]) -> List[int]:
    """
    Converts a translation vector to a list of 4 signed 16-bit integers.

    A translation vector is a three-dimensional vector that represents movement along the x, y, and z axes.
    This function converts such a vector to a list of four signed 16-bit integers.

    Args:
        translation (List[float]): The translation vector to convert.

    Returns:
        List[int]: The list of 4 signed 16-bit integers representing the translation.
    """

    # Start with the identifier value.
    result = [-32768]

    # Convert the translation vector to a list of signed 16-bit integers
    result.extend([clamp(int(x), -32768, 32767) for x in translation[1:4]])

    return result

def s16_scale(scale: List[float], bfloat: bool) -> List[int]:
    """
    Converts a scale vector to a list of 4 signed 16-bit integers or 
    fixed exponent 16-bit bfloats.

    This function converts a scale vector to a list of four signed 16-bit integers or 
    fixed exponent 16-bit bfloats.

    Args:
        scale (List[float]): The scale vector to convert.
        bfloat (bool): If True, convert to fixed exponent 16-bit bfloats. 
        Otherwise, convert to signed 16-bit integers.

    Returns:
        List[int]: The list of 4 signed 16-bit integers or 
        fixed exponent 16-bit bfloats representing the scale.
    """

    result = [-32767]

    if bfloat:
        # Convert the scale vector to a list of fixed exponent 16-bit bfloats
        result.extend([float_to_fixed_exponent_bfloat(x, 48.0, 132) for x in scale[1:4]])
    else:
        # Convert the scale vector to a list of signed 16-bit integers
        result.extend([clamp(int(clamp(x, -16.0, 16.0) * (32767.0 / 16.0)), -32768, 32767) for x in scale[1:4]])

    return result

def matrix_euler_vec3s(eulerAngle: List[float]) -> List[List[float]]:
    """
    This function takes in a list of Euler angles in degrees and returns a 4x4 rotation matrix.

    Args:
        eulerAngle (float): list of 3 Euler angles in degrees [x, y, z]

    Returns:
        List[List[float]]: 4x4 rotation matrix
    """
    # Convert Euler angles from degrees to radians
    degX = math.radians(eulerAngle[0])
    degY = math.radians(eulerAngle[1])
    degZ = math.radians(eulerAngle[2])

    # Calculate sine and cosine of the angles
    sx = math.sin(degX)
    cx = math.cos(degX)
    sy = math.sin(degY)
    cy = math.cos(degY)
    sz = math.sin(degZ)
    cz = math.cos(degZ)

    # Initialize the 4x4 rotation matrix
    dest = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 1]]

    # Calculate the elements of the rotation matrix
    dest[0][0] = (cy * cz)
    dest[0][1] = (cy * sz)
    dest[0][2] = -sy
    sxcz = (sx * cz)
    cxsz = (cx * sz)
    dest[1][0] = ((sxcz * sy) - cxsz)
    sxsz = (sx * sz)
    cxcz = (cx * cz)
    dest[1][1] = ((sxsz * sy) + cxcz)
    dest[1][2] = (sx * cy)
    dest[2][0] = ((cxcz * sy) + sxsz)
    dest[2][1] = ((cxsz * sy) - sxcz)
    dest[2][2] = (cx * cy)

    return dest

def quaternion_from_matrix(mtxf: List[List[float]]) -> List[float]:
    """
    This function takes in a 4x4 rotation matrix and returns a quaternion representing the same rotation.

    Args:
        mtxf (List[List[float]]): 4x4 rotation matrix

    Returns:
        List[float]: quaternion (wxyz)
    """
    # Calculate the trace of the matrix
    tr = mtxf[0][0] + mtxf[1][1] + mtxf[2][2]

    # Calculate the trace of the matrix
    tr = mtxf[0][0] + mtxf[1][1] + mtxf[2][2]

    # Calculate the quaternion based on the trace and the elements of the matrix
    if (tr > 0):
        # Case when the trace is positive
        S = math.sqrt(tr+1.0) * 2; # S=4*qw 
        qw = 0.25 * S
        qx = (mtxf[2][1] - mtxf[1][2]) / S
        qy = (mtxf[0][2] - mtxf[2][0]) / S
        qz = (mtxf[1][0] - mtxf[0][1]) / S

    elif ((mtxf[0][0] > mtxf[1][1]) and (mtxf[0][0] > mtxf[2][2])):
        # Case when the first diagonal element is the greatest
        S = math.sqrt(1.0 + mtxf[0][0] - mtxf[1][1] - mtxf[2][2]) * 2; # S=4*qx 
        qw = (mtxf[2][1] - mtxf[1][2]) / S
        qx = 0.25 * S
        qy = (mtxf[0][1] + mtxf[1][0]) / S
        qz = (mtxf[0][2] + mtxf[2][0]) / S

    elif (mtxf[1][1] > mtxf[2][2]):
        # Case when the second diagonal element is the greatest
        S = math.sqrt(1.0 + mtxf[1][1] - mtxf[0][0] - mtxf[2][2]) * 2; # S=4*qy
        qw = (mtxf[0][2] - mtxf[2][0]) / S
        qx = (mtxf[0][1] + mtxf[1][0]) / S
        qy = 0.25 * S
        qz = (mtxf[1][2] + mtxf[2][1]) / S

    else:
        # Case when the third diagonal element is the greatest
        S = math.sqrt(1.0 + mtxf[2][2] - mtxf[0][0] - mtxf[1][1]) * 2; # S=4*qz
        qw = (mtxf[1][0] - mtxf[0][1]) / S
        qx = (mtxf[0][2] + mtxf[2][0]) / S
        qy = (mtxf[1][2] + mtxf[2][1]) / S
        qz = 0.25 * S

    return [qw, qx, qy, qz]

def quaternion_from_euler(eulerAngle: List[float]) -> List[float]:
    """
    This function takes in a list of Euler angles in degrees and returns a quaternion representing the same rotation.

    Args:
        eulerAngle (float): list of 3 Euler angles in degrees [x, y, z]

    Returns:
        List[float]: quaternion (wxyz)
    """
    # Convert Euler angles to a 4x4 rotation matrix using vanilla math
    matrix = matrix_euler_vec3s(eulerAngle)
    
    # Convert the rotation matrix to a quaternion
    return quaternion_from_matrix(matrix)

def normalize_quaternion(quaternion: List[float]) -> List[float]:
    """
    Normalizes a quaternion to have a magnitude of 1.

    Args:
        quaternion (List[float]): The quaternion to normalize.

    Returns:
        List[float]: The normalized quaternion.
    """
    # Calculate the magnitude of the quaternion
    mag = math.sqrt(sum([x**2 for x in quaternion])) + 0.0001

    # Normalize the quaternion to have a magnitude of 1
    normalizedQuaternion = [x/mag for x in quaternion]

    return normalizedQuaternion

def quaternion_to_3_element(quaternion: List[float]) -> List[float]:
    """
    Converts a quaternion to a 3-element list.

    Args:
        quaternion (List[float]): The quaternion to convert.

    Returns:
        List[float]: The converted 3-element list.
    """
    normalizedQuaternion = normalize_quaternion(quaternion)

    # Apply the scaler´s sign
    if normalizedQuaternion[0] < 0:
        normalizedQuaternion = [-x for x in normalizedQuaternion]

    return normalizedQuaternion[1:4]

def quaternion_mul(a: List[float], b: List[float]):
    """
    Multiplies two quaternions a and b.

    The formula for the real part w of quaternion c is: 
    w = w1*w2 - x1*x2 - y1*y2 - z1*z2. 
    The formulas for the imaginary parts x, y, and z of quaternion c are: 
    x = w1*x2 + x1*w2 + y1*z2 - z1*y2; 
    y = w1*y2 - x1*z2 + y1*w2 + z1*x2; 
    z = w1*z2 + x1*y2 - y1*x2 + z1*w2

    Args:
        a (List[float]): list of length 4 representing quaternion a.
        b (List[float]): list of length 4 representing quaternion b.

    Return:
        List[int]: list of length 4 representing the product of a and b
    """

    w1, x1, y1, z1 = a
    w2, x2, y2, z2 = b

    w = w1*w2 - x1*x2 - y1*y2 - z1*z2
    x = w1*x2 + x1*w2 + y1*z2 - z1*y2
    y = w1*y2 - x1*z2 + y1*w2 + z1*x2
    z = w1*z2 + x1*y2 - y1*x2 + z1*w2
    
    return [w, x, y, z]

def u16_to_s16(uintValue: int):
    """
    Converts an unsigned 16-bit integer to a signed 16-bit integer.

    This function takes an unsigned 16-bit integer and converts it to a 
    signed 16-bit integer by checking if the sign bit is set and 
    subtracting 2^16 if it is.

    Args:
        uintValue (int): The unsigned 16-bit integer to convert.

    Returns:
        int: The converted signed 16-bit integer.
    """

    if (uintValue & 0x8000) != 0:  # Check if the sign bit is on
        return uintValue - (1 << 16)
    return uintValue

class Define:
    """
    Class to represent a define statement.
    """
    def __init__(self, name: str, values: List, macros: str):
        """
        Initializes a Define object.

        Args:
            name (str): name of the define statement
            values (List): Values of the define statement
            macros (str): Macro the define statement is under in
        """
        self.name = name
        self.values = values
        self.macros = macros

class ArrayOrStruct:  
    """
    Class to represent an array or struct in C/C++.
    """
    def __init__(self, keywords, type, name, values, ogString, isArray, file):
        """
        Initializes an ArrayOrStruct object.

        Args:
            keywords (List[str]): keywords used in the declaration
            type (str): type of the array or struct
            name (str): name of the array or struct
            values (List): values assigned to the array or struct
            ogString (str): original c declaration
            isArray (bool): indicates whether the object is an array (True) or struct (False)
            file (str): spath to the file in which the array or struct is declared
        """

        self.keywords = keywords
        self.type = type
        self.name = name
        self.values = values
        self.ogString = ogString
        self.isArray = isArray
        self.file = file

class GeoLayoutCommand:
    """
    This class represents a GeoLayout Node and its children.
    """
    def __init__(self, command, values, parent, children, macros, firstCommand):
        """
        Initializes a new instance of the GeoLayoutCommand class.

        Args:
            command (str): The geolayout node´s command.
            values (List): Values for the node.
            parent (GeoLayoutCommand): The parent of this command.
            children (List[GeoLayoutCommand]): The children of this command.
            macros (str): The macros this command is under in.
            firstCommand (GeoLayoutCommand): The first command in the geolayout.
        """

        self.firstCommand = firstCommand
        self.isReturn = False
        self.command = command
        self.values = values
        self.parent = parent
        self.children = children
        self.merge = False
        self.remove = False
        self.beforeChildMacros = ""
        self.afterChildMacros = ""
        self.macros = macros
    
    def read_geo_array(self, actor, geolayoutArray: ArrayOrStruct):
        """
        Reads a geolayoutArray and populates the instance variables of the current
        GeoLayoutCommand object based on its contents.

        Args:
            geolayoutArray (ArrayOrStruct): The geolayout´s array.
        """

        self.name = geolayoutArray.name

        # Extract the first element of the geolayoutArray
        firstDefine = geolayoutArray.values[0]

        # Set instance variables based on the first element
        self.command = firstDefine.name
        self.values = firstDefine.values
        self.parent = None
        self.macros = firstDefine.macros
        self.firstCommand = self

        self.actor = actor
        self.geolayoutArray = geolayoutArray

        self.boneIndexes = []
        self.cExport = ""

        self.isAnimatable = False
        self.needsReexport = False

        # Initialize parentGeoCommand to the current instance
        parentGeoCommand = self

        # Loop through the remaining elements of the geolayoutArray
        for define in geolayoutArray.values[1:]:
            if define.name == "GEO_OPEN_NODE":
                # If the current element is GEO_OPEN_NODE, create a child command
                # and set its macros to the macros of the current element
                if len(parentGeoCommand.children) > 0:
                    parentGeoCommand = parentGeoCommand.children[-1]
                    parentGeoCommand.beforeChildMacros = define.macros

            elif define.name == "GEO_CLOSE_NODE":
                # If the current element is GEO_CLOSE_NODE, set the afterChildMacros
                # of the parentGeoCommand and update the parentGeoCommand to its parent
                if parentGeoCommand.parent != None:
                    parentGeoCommand.afterChildMacros = define.macros
                    parentGeoCommand = parentGeoCommand.parent
    
            elif define.name == "GEO_RETURN":
                # If the current element is GEO_RETURN, set the isReturn flag to True
                self.isReturn = True
                continue

            elif define.name == "GEO_END":
                # If the current element is GEO_END, continue to the next element
                continue

            else:
                # If the current element is not GEO_OPEN_NODE, GEO_CLOSE_NODE,
                # GEO_RETURN, or GEO_END, create a child command and append it to
                # the parentGeoCommand's children list
                if define.name == "GEO_ANIMATED_PART":
                    self.isAnimatable = True

                child = GeoLayoutCommand(
                    define.name, define.values.copy(), parentGeoCommand, [], define.macros, self)

                parentGeoCommand.children.append(child)
    
    def find_bone_indexes(self, boneIndexArray: List):
        """
        This function finds the bone indexes for a given geolayout
        and appends them to the provided boneIndexArray in their in game sequence.
        
        Args:
            boneIndexArray (List[GeoLayoutCommand]): A list to which the bones will be appended.
        """

        # If the command is GEO_ANIMATED_PART, append self to boneIndexArray
        if self.command == "GEO_ANIMATED_PART":
            boneIndexArray.append(self)
        
        # If the command is GEO_SWITCH_CASE or GEO_BRANCH
        if self.command == "GEO_SWITCH_CASE" or self.command == "GEO_BRANCH":
            biggestSize = 0
            toAppend = None

            for child in self.children:
                childBoneIndexArray = []
                child.find_bone_indexes(childBoneIndexArray)
                size = len(childBoneIndexArray)
                
                # Find the child with the largest size
                if size > biggestSize:
                    toAppend = childBoneIndexArray
                    biggestSize = size
            
            # Extend boneIndexArray with the largest childBoneIndexArray
            if toAppend:
                boneIndexArray.extend(toAppend)
        
        # If none of the above conditions are met
        else:
            lastChild = None

            for child in self.children:
                # If the macro of the child is else, endif or none
                if child.macros in ["else", "endif", ""]:
                    if not lastChild or lastChild.command not in ["GEO_RENDER_CASE", "GEO_RENDER_RANGE"]:
                        child.find_bone_indexes(boneIndexArray)
                    lastChild = child

    def find_unused_bones(self):
        """
        This function finds unused bones in the geolayout and its children and sets 
        their merge and remove flags accordingly.
        """
        shouldMerge = False
        shouldRemove = False
        
        actor = self.firstCommand.actor
        settings = actor.settings
    
        mergeFirstBone = settings.get_geo_setting(actor.name, self.firstCommand.name, "CAN_MERGE_FIRST_BONE")
        removeChildlessNulls = settings.get_geo_setting(actor.name, self.firstCommand.name, "REMOVE_CHILDLESS_NULL_BONES")

        isFirstBone = (self.firstCommand.boneIndexes[0] == self)

        isAnimBone = (self.command == "GEO_ANIMATED_PART")

        # If the bone has an animation bone parent
        isParentAnimBone = self.parent and (self.parent.command == "GEO_ANIMATED_PART")

        if isAnimBone:
            isNull = self.values[4] == "NULL"
            childAmount = len(self.children)

            if removeChildlessNulls and (isNull and childAmount == 0):
                shouldRemove = True

            zeroTrans = self.values[1:4] == [0, 0, 0]
            
            if isNull:
                if isParentAnimBone and zeroTrans:
                    shouldMerge = True
                for child in self.children:
                    if child.command == "GEO_ANIMATED_PART" and (child.values[1:4] == [0, 0, 0]):
                        shouldMerge = True
                    else:
                        shouldMerge = False
                        break
            
            if isFirstBone and (childAmount > 1 or not mergeFirstBone):
                shouldMerge = False

        self.merge = shouldMerge
        self.remove = shouldRemove

        if self.merge or self.remove:
            self.firstCommand.needsReexport = True

        # Recursively call this function on all children of this object
        for child in self.children:
            child.find_unused_bones()
        
    def fix_geolayout(self):
        """
        This function fixes the geolayout of the bone and its children 
        by removing or merging bones as needed.
        """
        # Recursively call this function on all children of this bone
        for child in self.children.copy():
            child.fix_geolayout()

        # If this bone has no parent or is not in its parent's children list, return
        if not self.parent or self not in self.parent.children:
            return

        # Get the index of this bone in its parent's children list
        index = self.parent.children.index(self)

        # If this bone is flagged for removal and is in its parent's children list, remove it
        if self.remove and self in self.parent.children:
            self.parent.children.pop(index)

        elif self.merge:
            pos = index

            # Remove this bone from its parent's children list
            if self in self.parent.children:
                self.parent.children.pop(index)

            for child in self.children:
                # If the child is an animation bone, add this bone's translation values to the 
                # child's translation values
                if child.command == "GEO_ANIMATED_PART":
                    child.values[1] += self.values[1]
                    child.values[2] += self.values[2]
                    child.values[3] += self.values[3]

                # Set the child's parent to this bone's parent and insert the child into the 
                # parent's children list at the current position
                child.parent = self.parent
                self.parent.children.insert(pos, child)

                pos += 1

    def reexport(self):
        """
        Converts the geolayout to a C format and rewrites the original source array.
        """

        cExport = f"const GeoLayout {self.geolayoutArray.name}[] = {{\n{self.to_c_data()[0]}}};"

        # Replace original geolayout string with cExport in geolayoutTexts
        updatedFileText = self.actor.geolayoutTexts[self.geolayoutArray.file].replace(self.geolayoutArray.ogString, cExport)
        self.actor.geolayoutTexts[self.geolayoutArray.file] = updatedFileText

        # Rewrite geolayout file with updated geolayoutTexts
        self.actor.rewrite_files(self.actor.geolayoutTexts, True, self.geolayoutArray.file)   

    def change_macro_state(self, string, macros, newMacros):
        """
        Change macro state if newMacros is different from macros.
        """
        if macros != newMacros:
            if len(newMacros) > 2:
                string += f"#{newMacros}\n"
            return string, newMacros
        else:
            return string, macros

    def to_c_data(self, string="", macros="", level=1):
        """
        Convert geolayout to in c data representation.
        """
        tabs = "\t" * level

        # Change macro state
        string, macros = self.change_macro_state(string, macros, self.macros)

        # Convert values to string
        values = ""
        if self.values:
            values = ', '.join([str(elem) for elem in self.values])

        # Add command and values to string
        string += f"{tabs}{self.command}({values}),"

        # Add merge or remove comment if applicable
        if self.merge:
            string += "// Merge"    
        elif self.remove:
            string += "// Remove"

        string += "\n"

        # Add children to string
        if self.children:
            # Change macro state before children
            string, macros = self.change_macro_state(string, macros, self.beforeChildMacros)
            string += f"{tabs}GEO_OPEN_NODE(),\n"

            # Recursively call this function on all children of this object
            for child in self.children:
                string, macros = child.to_c_data(string, macros, level + 1)

            # Change macro state after children
            string, macros = self.change_macro_state(string, macros, self.afterChildMacros)
            string += f"{tabs}GEO_CLOSE_NODE(),\n"
            
            # Add return or end if at top level
            if level == 1:
                if self.isReturn:
                    string += f"{tabs}GEO_RETURN(),\n"
                else:
                    string += f"{tabs}GEO_END(),\n"    

        return string, macros

def find_all_arrays_in_text(string: str, file: str, arrays: dict):
    """
    Parses the provided string for arrays and populates the provided arrays dictionary with the parsed data.
    
    Args:
        string (str): The string to be parsed for arrays.
        file (str): The name of the file being parsed (for error messages).
        arrays (dict): A dictionary object to store the arrays found in the string.
    """

    charIndex = 0
    end = len(string)
    readingKeywords = True
    keywordsStart = 0
    keywordStart = 0
    valueStart = 0
    isArray = False
    readingValues = False

    commentStart = 0
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
            # Single line comment detected
            commentStart = charIndex
            inComment = True
        
        if previousChar == "/" and char == "*":
            # Multi-line comment detected
            commentStart = charIndex
            inComment = True
            multiLineCommment = True

        if inComment:
            # Check if the comment has ended
            if char == "\n" and not multiLineCommment:
                inComment = False
            
            # Check if a comment has ended in a multi-line comment
            if multiLineCommment and previousChar == "*" and char == "/":
                if commentStart > keywordStart < charIndex:
                    keywordStart = charIndex + 1
                
                if commentStart > valueStart < charIndex:
                    valueStart = charIndex + 1
                
                if commentStart > defineStart < charIndex:
                    defineStart = charIndex + 1
                    
                inComment = False
                multiLineCommment = False 
        
        # If not in comment continue parsing
        if not inComment:
            if readingMacro:
                # Check for macro continuation symbols
                if char == "&" or char == "|":
                    readMacro = True

                # Check for the end of the macro name
                if (previousChar == " " or char == "\n" or char == "\t") and (char != "&" and char != "|"):
                    if readMacroName:
                        # Check if the macro is an ifdef or ifndef
                        macroName = string[macroStart:charIndex].strip()
                        if macroName == "ifdef" or macroName == "ifndef":
                            readMacro = True
                            readMacroName = False

                    if not readMacro:
                        # Save the macro name and reset the macro state
                        currentMacro = string[macroStart:charIndex].strip()                     
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
                    # Start of macro detected
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
                        if not inParen:
                            defineStart = charIndex + 1
                            define = Define(string_to_value(string[valueStart:charIndex].strip()), [], currentMacro)
                            valueStart = defineStart
                            isDefine = True
                        inParen = True
                    elif char == ')':
                        inParen = False
                    elif char == ',': 
                        if not inParen:
                            if isDefine == True:
                                values.append(define)
                                isDefine = False
                            else:
                                values.append(string_to_value(string[valueStart:charIndex].strip()))
                        valueStart = charIndex + 1 
                
                # In between stage, = or [] has been reached so we are no longer reading keywords but we are still not reading values until the first {
                else:
                    if char == "{":
                        readingValues = True
                        valueStart = charIndex + 1
                
        previousChar = char
        charIndex += 1

class OldAnimation:
    def __init__(self, actor):
        self.actor = actor

    def read_animation_array(self, array: ArrayOrStruct, animFilesArrays: dict):
        """
        This function reads an animation array and extracts relevant information from it.

        Args:
            array (ArrayOrStruct): The animation array to be read.
            animFilesArrays (dict): A list of animation files arrays.
        """
        # Set the structArray and name attributes
        self.structArray = array
        self.name = array.name
        print_info(f"Reading animation {self.name}´s struct.")

        # Get the flags as a value
        flagsAsValue = array.values[0]

        flags = {
            0: "ANIM_FLAG_NOLOOP",
            1: "ANIM_FLAG_FORWARD",
            2: "ANIM_FLAG_NO_ACCEL",
            3: "ANIM_FLAG_HOR_TRANS",
            4: "ANIM_FLAG_VERT_TRANS",
            5: "ANIM_FLAG_DISABLED",
            6: "ANIM_FLAG_NO_TRANS",
        }

        # Set the flags attribute to a list of flags that are true for the given value
        self.flags = [flag for bit, flag in flags.items() if is_bit_true(flagsAsValue, bit)]

        # Set other attributes based on values in the array
        self.animYTransDivisor = array.values[1]
        self.startFrame = array.values[2]
        self.loopStart = array.values[3]
        self.loopEnd = array.values[4]
        self.valuesArray = animFilesArrays[array.values[6]]
        self.indexesArray = animFilesArrays[array.values[7]]
        self.length = array.values[8]

    def fetch_animation_data(self):
        """
        Fetches animation data for the animation. 
        If the animation data does not exist in the actor's old animation data, 
        it creates a new OldAnimationData object and fetches the animation indexes and values. 
        It then stores the animation data in the actor's old animation data dictionary.
        """
        # Check if the animation data does not exist in the actor's old animation data dictionary
        if self.indexesArray.name not in self.actor.oldAnimData:

            animData = OldAnimationData(self, self.valuesArray, self.indexesArray, self.actor)
            animData.fetch_animation_indexes()
            animData.fetch_animation_values()

            self.actor.oldAnimData[self.indexesArray.name] = animData

        self.animData = self.actor.oldAnimData[self.indexesArray.name]

class OldAnimationData:
    def __init__(self, anim: OldAnimation, valuesArray: ArrayOrStruct, indexesArray: ArrayOrStruct, actor):
        """
        Initializes the object with the given parameters.

        Args:
            anim (OldAnimation): Animation struct that uses the animation data.
            valuesArray (ArrayOrStruct): Array of values.
            indexesArray (ArrayOrStruct): Frame amount and value offset pairs array.
            actor (Actor): Actor which includes this animation data.
        """
        self.anim = anim
        self.valuesArray = valuesArray
        self.indexesArray = indexesArray
        self.actor = actor

    def fetch_animation_indexes(self):
        """
        Fetches the animation indexes and calculates the maximum frame and bone count.
        """
        # Get the animation indexes table
        indexes = self.indexesArray.values

        # Calculate the number of bone elements
        self.boneElementCount = len(indexes) // 2

        # Calculate the number of bones
        self.boneCount = (clamp((self.boneElementCount // 3) - 1, 0, math.inf))

        print_info(f"\
Reading animation indexes from {self.indexesArray.name}. \
Bone elements: {self.boneElementCount}. \
Bones: {self.boneCount}")

        self.elements = []
        self.maxFrame = 0

        # Iterate over the indexes to read the maximum frames and value offsets
        # and allocate value lists.
        for i in range(0, len(indexes), 2):
            maxFrame = indexes[i]
            offset = indexes[i + 1]

            if maxFrame > self.maxFrame:
                self.maxFrame = maxFrame

            self.elements.append([offset, maxFrame, []])
        
    def fetch_animation_values(self):
        """
        Fetches the animation values and populates the elements list with 
        their frame data.
        """

        print_info(f"Reading animation values from {self.valuesArray.name}. Frames: {self.maxFrame}")

        # Get the animation values table
        values = self.valuesArray.values

        for boneElement in self.elements:
            # Iterate over the frames for the current bone element
            for j in range(boneElement[1]):
                pos = boneElement[0] + j

                # Append the value for the current frame to the bone element's frames data list
                boneElement[2].append(values[pos])

class NewAnimationData:
    """
    A class to represent new animation data for an actor.
    """

    def __init__(self, actor, anim):
        """
        Initializes a NewAnimationData object with an actor and an animation.

        Args:
            actor (Actor): The actor associated with the animation data.
            anim (Animation): The animation associated with the animation data.
        """

        self.actor = actor
        self.anim = anim
        self.newFormatArray = None

    def find_geo(self):
        """
        Finds the geolayout for the animation data based on the 
        actor's settings and geolayouts.
        """
        # Get the actor's settings and names
        settings = self.actor.settings
        actorName = self.actor.name
        animName = self.anim.name

        # Get the specific geolayout for the animation
        specificGeoSetting = settings.get_anim_setting(actorName, animName, "SPECIFIC_GEO")

        if specificGeoSetting == "FIND_AUTO":
            closestGeolayout = None
            maxBoneDiff = settings.get_anim_setting(actorName, animName, "MAX_BONE_DIFFERENCE")
    
            for geolayout in self.actor.geolayouts:
                boneDiff = abs(len(geolayout.boneIndexes) - self.oldBoneCount)

                if boneDiff <= maxBoneDiff:
                    closestGeolayout = geolayout
                    maxBoneDiff = boneDiff + 1

                if boneDiff == 0:
                    break
            self.geolayout = closestGeolayout
        else:
            for geolayout in self.actor.geolayouts:
                if geolayout.geolayoutArray.name == specificGeoSetting:
                    self.geolayout = geolayout
                    break
            
        if not self.geolayout:
            print_error(f"\
Cannot find geolayout for {self.name}. (Actor name: {actorName}, Anim name: {animName}), \
Old bone count: {self.oldBoneCount})")

    def from_new_format(self, array: ArrayOrStruct):
        """
        Converts animation data from a new format array.

        Args:
            array (ArrayOrStruct): The animation data´s array.
        """

        self.newFormatArray = array
        self.name = array.name

        # Assume the new format is in 4 element format.
        elementCount = self.anim.elementCount
        boneCount = elementCount // 4
        self.maxFrame = self.anim.loopEnd

        elementIndex = 0

        self.frames = []
        for frameNum in range(self.maxFrame):
            frame = []
            boneIndex = 0

            for element in range(boneCount):
                value = array.values[elementIndex:elementIndex + 4]
                
                # Handle scale vector.
                if value[0] == -32767:
                    value[1:4] = [x * (16.0 / 32767.0) for x in value[1:4]]
                # This ignores translation since it doesn´t get scaled by any value
                # If its not translation or scale handle rotation
                elif value[0] != -32768:
                    value = [x / 32765.0 for x in value]
                    boneIndex += 1

                frame.append(value)
                elementIndex += 4

            self.frames.append(frame)

        self.oldBoneCount = boneIndex

    def from_old_format(self, array: ArrayOrStruct):
        """
        Converts animation data from an old format array.

        Args:
            array (ArrayOrStruct): The animation data´s array.
        """
        self.oldAnimData = array
        self.valuesArray = array.valuesArray
        self.indexesArray = array.indexesArray

        self.name = f"{self.valuesArray.name}_data"
        self.maxFrame = array.maxFrame

        self.oldBoneCount = array.boneCount

        print_info(f"\
Converting animation data of {array.valuesArray.name} to new format. \
Max frames: {array.maxFrame}. \
Element amount: {len(array.elements)}. \
Flags: {array.anim.flags}")

        self.frames = []
        values = []
        frame = 0
        while frame < self.maxFrame:
            boneValues = []
            applyRoot = True

            for boneElement in array.elements:
                frames = boneElement[2]
                maxFrame = boneElement[1]

                # Append the value for the current frame (or the last frame of the element) 
                # to the values list
                clampedFrame = clamp(frame, 0, maxFrame - 1)
                value = frames[clampedFrame]
                values.append(value)
                
                if len(values) == 3:
                    if applyRoot:
                        # Convert values to signed 16-bit integers and append to bone values list
                        boneValues.append([-32768, u16_to_s16(values[0]), u16_to_s16(values[1]), u16_to_s16(values[2])])
                        applyRoot = False
                    else:
                        # Convert values to euler angles and then to quaternions
                        eulerAngle = [angle(values[0]), angle(values[1]), angle(values[2])]
                        quaternion = quaternion_from_euler(eulerAngle)

                        boneValues.append(quaternion)

                    values.clear()

            self.frames.append(boneValues)
            frame += 1

    def fix_animation(self):
        """
        Fix animation data for a character model based on animation settings.

        This modifies translation, scale, and quaternion values for each bone.
        Some of the changes are definied in the settings file such as applying horizontal or 
        vertical translation flags, only using translation and or scale in the first bone,
        disabling scale, etc etc. It also fixes the animation to be in line with the fixed geolayout.
        It also updates the variables boneCount, elementCount, and framesSize.
        """

        self.find_geo()
        
        settings = self.actor.settings
        actorName = self.actor.name
        animName = self.anim.name

        zeroOutHor = False
        zeroOutVer = False
        noTrans = False

        useThreeElement = settings.get_anim_setting(actorName, animName, "USE_THREE_ELEMENT_QUATERNIONS")

        if settings.get_anim_setting(actorName, animName, "APPLY_TRANS_FLAGS"):
            if settings.get_anim_setting(actorName, animName, "APPLY_HORIZONTAL_TRANS_FLAG"):
                if self.anim.flags and "ANIM_FLAG_HOR_TRANS" in self.anim.flags:
                    zeroOutHor = True
            
            if settings.get_anim_setting(actorName, animName, "APPLY_VERTICAL_TRANS_FLAG"):
                if self.anim.flags and "ANIM_FLAG_VERT_TRANS" in self.anim.flags:
                    zeroOutVer = True
            
            if settings.get_anim_setting(actorName, animName, "APPLY_NO_TRANS_FLAG"):
                if self.anim.flags and "ANIM_FLAG_NO_TRANS" in self.anim.flags:
                    noTrans = True

        print_info(f"\
Fixing animation data ({self.name}). \
Animation flags - {self.anim.flags} \
Zero Out Horizontal - {zeroOutHor}, Zero Out Vertical - {zeroOutVer}, No Trans- {noTrans}")
    
        transBoneSetting = self.actor.settings.get_anim_setting(self.actor.name, self.anim.name, "TRANSLATE_BONE")
        scaleBoneSetting = self.actor.settings.get_anim_setting(self.actor.name, self.anim.name, "SCALE_BONE")

        self.hasScale = False

        newFrames = []
        for frame in self.frames:
            newFrame = []
            boneIndex = 0
            realBoneCount = 0

            isTrans, isRotation, isScale = False, False, False

            for bone in self.geolayout.boneIndexes:
                bone.translation = None
                bone.quaternion = None
                bone.scale = None

            for elementIndex in range(len(frame)):
                element = frame[elementIndex]

                if element[0] == -32768:    # Trans
                    if isRotation:
                        boneIndex += 1
                    isTrans, isRotation, isScale = True, False, False
                elif element[0] == -32767:  # Scale
                    if not (isTrans) and isRotation:
                        boneIndex += 1
                    isScale, isTrans, isRotation = True, False, False
                else:                       # Rotation
                    if not (isTrans or isScale) and isRotation:
                        boneIndex += 1
                    isRotation, isTrans, isScale = True, False, False

                if not self.geolayout or boneIndex >= len(self.geolayout.boneIndexes):
                    bone = GeoLayoutCommand("Unknown bone", ["Unknown bone"], None, ["Unknown bone"], "Unknown bone", None)
                    bone.translation = None
                    bone.quaternion = None
                    bone.scale = None
                else:
                    bone = self.geolayout.boneIndexes[boneIndex]
                
                if isTrans:
                    if not noTrans and not (transBoneSetting == "FIRST" and boneIndex > 0) and not transBoneSetting == "NONE":
                        if zeroOutHor:
                            element[0] = 0
                            element[2] = 0
                        if zeroOutVer:
                            element[1] = 0

                        bone.translation = element

                elif isScale:
                    if not (scaleBoneSetting == "FIRST" and boneIndex != 0) and scaleBoneSetting != "NONE":
                        bone.scale = element
                        self.hasScale = True

                elif isRotation:
                    bone.quaternion = element
                
                parentIsMergeBone = (bone.parent and bone.parent.merge)
                realBone = not (bone.merge or bone.remove)
                
                if parentIsMergeBone:
                    if bone.parent.translation:
                        pTrans = bone.parent.translation
                        if bone.translation:
                            trans = bone.translation
                            bone.translation = [-32768, trans[1] + pTrans[1], trans[2] + pTrans[2], trans[3] + pTrans[3]]
                        else:
                            bone.translation = pTrans

                    if bone.parent.quaternion:
                        pQuat = bone.parent.quaternion
                        if bone.quaternion:
                            bone.quaternion = quaternion_mul(bone.quaternion, pQuat)
                        else:
                            bone.quaternion = pQuat

                    if bone.parent.scale:
                        pScale = bone.parent.scale
                        if bone.scale:
                            scale = bone.scale
                            bone.scale = [-32767, scale[1] * pScale[1], scale[2] * pScale[2], scale[3] * pScale[3]]
                        else:
                            bone.scale = pScale

                if realBone:
                    if bone.translation:
                        newFrame.append(bone.translation)
                        bone.translation = None

                    if bone.scale:
                        newFrame.append(bone.scale)
                        bone.scale = None

                    if bone.quaternion:
                        quaternion = bone.quaternion
                        if useThreeElement:
                            quaternion = quaternion_to_3_element(bone.quaternion)
                        newFrame.append(quaternion)
                        realBoneCount += 1
                        bone.quaternion = None

            newFrames.append(newFrame)
        
        self.boneCount = realBoneCount
        self.frames = newFrames
        self.elementCount = sum(len(transform) for transform in self.frames[0])
        self.framesSize = self.elementCount * self.maxFrame

    def generate_frame_data_to_c(self):
        settings = self.actor.settings
        actorName = self.actor.name
        animName = self.anim.name

        bfloat = settings.get_anim_setting(actorName, animName, "USE_FIXED_EXPONENT_BFLOATS")

        framesString = "\n/*"

        if self.geolayout:
            framesString += f"\
Detected geolayout: {self.geolayout.geolayoutArray.name}\n\
Bones in detected geolayout: {len(self.geolayout.boneIndexes)}\n"
        framesString += f"\
Bone count in old animation: {self.oldBoneCount}\n\
Bone count in new animation: {self.boneCount}\n"

        if not self.newFormatArray:
            framesString += f"\
Original value array: {self.valuesArray.name}\n\
Original indexes array: {self.indexesArray.name}\n"
        
        framesString += f"\
{(self.framesSize * 2) / 1000.0} kb*/\n{{\n"

        for frameIndex in range(len(self.frames)):
            framesString += f"\t/*Frame: {frameIndex}*/\n"
            frame = self.frames[frameIndex]

            for transform in frame:
                framesString += "\t\t"
                if transform[0] == -32768:
                    s16Transform = s16_translation(transform)  
                    framesString += f"/*Translation:*/ \t"
                elif transform[0] == -32767:      
                    s16Transform = s16_scale(transform, bfloat)  
                    framesString += f"/*Scale:*/ \t"
                else:
                    s16Transform = s16_quaternion(transform, bfloat)
                    framesString += "\t"

                framesString += f"{', '.join([str(x) for x in s16Transform])}, //{transform}\n"
            framesString += "\n"
        
        framesString += "}"
        return framesString
    
    def convert_frame_data_to_c(self):
        dataArray = f"static const s16 {self.name}[{self.framesSize}] = {self.generate_frame_data_to_c()};"        

        if self.newFormatArray:
            self.actor.animFilesTexts[self.newFormatArray.file] = self.actor.animFilesTexts[self.newFormatArray.file].replace(self.newFormatArray.ogString, dataArray)
            self.actor.rewrite_files(self.actor.animFilesTexts, False, self.newFormatArray.file)   
        else:
            self.actor.animFilesTexts[self.valuesArray.file] = self.actor.animFilesTexts[self.valuesArray.file].replace(self.valuesArray.ogString, dataArray)
            self.actor.animFilesTexts[self.indexesArray.file] = self.actor.animFilesTexts[self.indexesArray.file].replace(self.indexesArray.ogString, "")
            self.actor.rewrite_files(self.actor.animFilesTexts, False, self.valuesArray.file, self.indexesArray.file)   

class NewAnimation:
    def __init__(self, actor):
        self.actor = actor

    def from_old_format(self, oldAnim):
        print_info(f"Converting animation {oldAnim.name} to new format.")
        self.oldAnim = oldAnim
        self.originStruct = oldAnim.structArray

        self.animData = self.actor.newAnimData[oldAnim.indexesArray.name]

        self.name = oldAnim.name
        
        self.frameArrayName = f"{oldAnim.valuesArray.name}_data"
        self.loopEnd = clamp(oldAnim.loopEnd, 0, self.animData.maxFrame)
        self.loopStart = oldAnim.loopStart
        self.startFrame = oldAnim.startFrame
        self.animYTransDivisor = oldAnim.animYTransDivisor

        self.flags = self.oldAnim.flags

    def from_new_format(self, animStruct):
        self.name = animStruct.name

        values = {}
        for item in animStruct.values:
            if isinstance(item, Define):
                values["flags"] = item.values[0].strip().split('|')
                continue
            key, value = item.split(" = ")
            values[key.strip()[1:]] = string_to_value(value.strip())
        
        self.frameArrayName = values["frames"]
        self.frameArray = self.actor.animFilesArrays[self.frameArrayName]
        self.originStruct = animStruct

        self.startFrame = values["startFrame"]
        self.loopStart = values["loopStart"]
        self.loopEnd = values["loopEnd"]
        self.animYTransDivisor = values["animYTransDivisor"]

        self.elementCount = values["elementCount"]

        flags = values["flags"]
        if flags == 0:
            self.flags = []
        elif isinstance(flags, List):
            self.flags = flags
        else:
            self.flags = [flags]

        self.fetch_new_format_anim_data()
    
    def fix_format(self):
        if self.flags:
            flagsToRemove = self.actor.settings.get_anim_setting(self.actor.name, self.name, "FLAGS_TO_REMOVE")
            self.flags = [x for x in self.flags if x not in flagsToRemove]

            if self.animData.hasScale:
                scaleFlag = "ANIM_FLAG_HAS_SCALE"
                if scaleFlag not in self.flags:
                    self.flags.append(scaleFlag)

    def fetch_new_format_anim_data(self):
        if self.frameArrayName not in self.actor.newAnimData:
            animData = NewAnimationData(self.actor, self)
            animData.from_new_format(self.frameArray)
            self.actor.newAnimData[self.frameArrayName] = animData

        self.animData = self.actor.newAnimData[self.frameArrayName]

    def generate_struct_data_to_c(self, isDma=False, isPerFrame = False, animTableStructName = "", animTableName = ""):
        frameArrayName = self.frameArrayName
        if isDma:
            if isPerFrame:
                frameArrayName = f"(s16*) (offsetof(struct {animTableStructName}, {frameArrayName}))"
            else:
                frameArrayName = f"(s16*) (offsetof(struct {animTableStructName}, {frameArrayName}) - offsetof(struct {animTableStructName}, {self.name}))"

        if self.flags:
            if len(self.flags) == 1:
                c_flags = self.flags[0]
            else:
                c_flags = " | ".join(self.flags)
                c_flags = f"({c_flags})"
        else:
            c_flags = "0"

        return f"\
{{\n\
\t.frames = {frameArrayName},\n\
\t.startFrame =  {str(self.startFrame)},\n\
\t.loopStart = {str(self.loopStart)},\n\
\t.loopEnd = {str(self.loopEnd)},\n\
\t.animYTransDivisor = {str(self.animYTransDivisor)},\n\
\t.elementCount = {str(self.animData.elementCount)},\n\
\t.flags = {c_flags},\n\
}}"    
    
    def convert_anim_to_c(self):
        print_info(f"Converting animation {self.name} to c.")
        
        structString = f"static const struct Animation {self.name}"        

        if self.originStruct.isArray:
            structString += "[]"

        structString += f" =\n{self.generate_struct_data_to_c()};"

        self.actor.animFilesTexts[self.originStruct.file] = self.actor.animFilesTexts[self.originStruct.file].replace(self.originStruct.ogString, structString)
        self.actor.rewrite_files(self.actor.animFilesTexts, False, self.originStruct.file)   

class Actor:

    def rewrite_files(self, filesText: dict, useOld: bool, *files: List[str]):
        """
        This function rewrites the given files with the given animation files text.

        Args:
            filesText (dict): Containins the file´s text.
            useOld (bool): Indicates whether to make an .old file by default.
            *files (List[str]): All the file paths to be rewritten.
        """
        conversionComment = "// Converted by quaternion_anims.py (remove this line to have the file be reconverterd.)\n"

        # Loop through each file path
        for filePath in files:
            # Check if the file path does not end with ".old"
            if filePath[-4:] != ".old":
                if useOld or self.settings.get_global_setting("ALWAYS_USE_DOT_OLD"):
                    oldFilePath = f"{filePath}.old"

                    if not os.path.exists(oldFilePath) or not conversionComment in filesText[filePath]:
                        os.rename(filePath, oldFilePath)

                print_info(f"File {filePath} being rewritten.")

                # Open the new file in write mode
                with open(filePath,"w") as file:
                    # Check if the conversion comment is not in the text
                    if not conversionComment in filesText[filePath]:
                        filesText[filePath] = f"{conversionComment}{filesText[filePath]}"

                    # Write the updated text to the file
                    file.write(filesText[filePath])

    def reexport_dma_animations(self):
        """
        This generates an animation table for dma animations. 
        It generates the necessary includes, animation table struct and the animation table data.
        Finally it writes everything to the DMA export path specified in the actor's settings.
        """
        # Get animation table settings from the actor settings
        animTableName = self.settings.get_actor_setting(self.name, "DMA_ANIM_TABLE_NAME")
        animTableStructName = self.settings.get_actor_setting(self.name, "DMA_ANIM_TABLE_STRUCT_NAME")
        perFrameData = self.settings.get_actor_setting(self.name, "PER_FRAME_DMA")
        dmaExportPath = self.settings.get_actor_setting(self.name, "DMA_EXPORT_PATH")

        animAmount = len(self.newAnims)

        # Defines necessary headers
        includes = '\
#include "game/memory.h"\n\
#include <stddef.h>\n'

        # Create a list to store animation references
        animReferences = []
        for animIndex in range(len(self.newAnims)):
            anim = self.newAnims[animIndex]
            # Append animation name to the list
            animReferences.append(f"\tALIGNED16 struct Animation {anim.name}")
            # Check if the next animation data name is different from the current one
            if animIndex + 1 >= len(self.newAnims) or self.newAnims[animIndex + 1].animData.name != anim.animData.name:
                # Append animation data name and size to the list
                animReferences.append(f"\tALIGNED16 s16 {str(anim.animData.name)}[{anim.animData.framesSize}]")

        animReferences = ";\n".join(animReferences)

        # Create the animation table structure string
        animTableStruct = f"\
const struct {animTableStructName}\n\
{{\n\
\tu32 numEntries;\n\
\tconst struct Animation *addrPlaceholder;\n\
\tstruct OffsetSizePair entries[{animAmount}];\n\
{animReferences};\n\
}}\n"

        # Initialize sizes and offsets string
        sizesAndOffsets = ""
        for anim in self.newAnims:

            offset = f"\
offsetof(struct {animTableStructName}, {anim.name})"

            if perFrameData:
                size = f"\
sizeof({animTableName}.{anim.name})"
            else:
                size = f"\
offsetof(struct {animTableStructName}, {anim.animData.name}) + sizeof({animTableName}.{anim.animData.name}) - offsetof(struct {animTableStructName}, {anim.name})"

            # Append sizes and offsets to the string
            sizesAndOffsets += f"\t{{{offset}, {size}}},\n"

        animDataStrings = []
        for animIndex in range(len(self.newAnims)):
            anim = self.newAnims[animIndex]
            # Append the generated animation struct code to the list
            animDataStrings.append(anim.generate_struct_data_to_c(True, perFrameData, animTableStructName, animTableName))
            # Check if the next animation data name is different from the current one
            if animIndex + 1 >= len(self.newAnims) or self.newAnims[animIndex + 1].animData.name != anim.animData.name:
                # Append the generated frame data code to the list
                animDataStrings.append(anim.animData.generate_frame_data_to_c())

        animationData = ",\n".join(animDataStrings)

        # Create the animation table string
        animTable = f"\
{animTableName} = \n\
{{\n\
\t{animAmount},\n\
\tNULL,\n\
{{{sizesAndOffsets}}},\n\
{animationData}\n\
}};\n"

        with open(dmaExportPath, 'w') as file:
            file.write(includes)
            file.write(animTableStruct)
            file.write(animTable)
    
    def reexport_normal_animations(self):
        """
        Re-export normal animations by converting them to C and re-exporting them.
        """

        for anim in self.newAnims:
            anim.convert_anim_to_c()

        for animData in self.newAnimData.values():
            animData.convert_frame_data_to_c()

    def reexport_animations(self):
        """
        If the actor uses DMA, re-export in the DMA table format. 
        Otherwise, re-export normal animations.
        """

        if self.settings.get_actor_setting(self.name, "IS_DMA"):
            self.reexport_dma_animations()
        else:
            self.reexport_normal_animations()

    def update_animation_format(self):
        """
        Update the animation format from old to new by converting the old animation data to
        the new animation data format.
        """

        for oldAnimDataKey in self.oldAnimData.keys():
            oldAnimData = self.oldAnimData[oldAnimDataKey]

            newAnimData = NewAnimationData(self, oldAnimData.anim)
            newAnimData.from_old_format(oldAnimData)
            self.newAnimData[oldAnimDataKey] = newAnimData

    def fix_animation_data(self):
        """
        Fix animation data by calling the fix_animation method on each new animation data object.
        This includes fixing the animation to work with updated geo´s.
        """
        
        for newAnimData in self.newAnimData.values():
            newAnimData.fix_animation()

    def fix_animations(self):
        """
        Fix animation headers by converting old animation headers to new format and 
        fixing their flags.
        """

        for anim in self.oldAnims:
            newAnim = NewAnimation(self)
            newAnim.from_old_format(anim)
            self.newAnims.append(newAnim)
        
        for anim in self.newAnims:
            anim.fix_format()

    def fix_geos(self):
        """
        Fix geolayouts by checking if they need to be re-exported and, if so,
        fixing their geolayout and re-exporting them.
        """

        for geolayout in self.geolayouts:
            if not geolayout.needsReexport:
                continue

            geolayout.fix_geolayout()
            geolayout.reexport()

    def read_arrays_in_file(self, path: str, filesText: dict, arrays: dict):
        """
        Read arrays in a file and store them in the given dictionary.

        Args:
            path (str): The path to the file to read.
            filesText (dict): A dictionary to store the file text in.
            arrays (dict): A dictionary to store the arrays in.
        """

        # Comment to indicate that the file has been converted by quaternion_anims.py
        conversionComment = "// Converted by quaternion_anims.py (remove this line to have the file be reconverterd.)"

        with open(path, "r") as f:
            fileText = f.read()
            # Check if the conversion comment is present in the file text
            if conversionComment in fileText:
                return

            filesText[path] = fileText

            find_all_arrays_in_text(fileText, path, arrays)

    def read_geolayouts(self, geoPath: str):
        """
        Read geolayouts from a file and store them in the geolayouts list.

        Args:
            geoPath (str): The path to the file to read.
        """
        self.read_arrays_in_file(geoPath, self.geolayoutTexts, self.geolayoutArrays)

        for geolayoutArray in self.geolayoutArrays.values():

            geolayout = GeoLayoutCommand("", [], None, [], "", None)
            geolayout.read_geo_array(self, geolayoutArray)

            if not geolayout.isAnimatable:
                continue
            
            geolayout.find_bone_indexes(geolayout.boneIndexes)
            geolayout.find_unused_bones()

            self.geolayouts.append(geolayout)

    def read_new_animation(self, array: ArrayOrStruct):
        """
        Read a new animation from an array and append it to the newAnims list.

        Args:
            array (ArrayOrStruct): The array to read the animation from.
        """

        anim = NewAnimation(self)
        anim.from_new_format(array)

        self.newAnims.append(anim)

    def read_old_animation(self, array: ArrayOrStruct):
        """
        Read an old animation from an array and append it to the oldAnims list.

        Args:
            array (ArrayOrStruct): The array to read the animation from.
        """

        anim = OldAnimation(self)
        anim.read_animation_array(array, self.animFilesArrays)
        anim.fetch_animation_data()

        self.oldAnims.append(anim)

    def read_animation_data(self):
        """
        Read animation data from arrays in the animFilesArrays dictionary.
        """

        for array in self.animFilesArrays.values():
            if array.type == "Animation":
                print_info(f"Processing animation {array.name}")
                # Check if the length of the values in the array is 7
                if len(array.values) == 7:
                    self.read_new_animation(array)
                else:
                    self.read_old_animation(array)

    def search_through_animations(self):
        """
        Search through animations in the animPath and read arrays from files.
        """

        if os.path.isfile(self.animPath):
            self.read_arrays_in_file(self.animPath, self.animFilesTexts, self.animFilesArrays)
        elif os.path.isdir(self.animPath):
            # Get a list of file names in the directory
            fileNames = os.listdir(self.animPath)

            # Sort the file names alphabetically.
            fileNames.sort()

            for name in fileNames:
                # Check if the name is "data.inc.c", "table.inc.c", or ends with ".old"
                if name == "data.inc.c" or name == "table.inc.c" or name[-4:] == ".old":
                    continue

                # Join the animPath with the file name to get the full path to the file
                animPath = os.path.join(self.animPath, name)

                self.read_arrays_in_file(animPath, self.animFilesTexts, self.animFilesArrays)

    def process_animations(self):
        """
        Process animations by searching through them and reading animation data.
        """

        self.search_through_animations()
        self.read_animation_data()

    def read_data(self):
        """
        Read data by processing animations and reading geolayouts.
        """

        self.process_animations()

        oldGeoPath = f"{self.geolayoutPath}.old"

        if os.path.exists(oldGeoPath):
            self.read_geolayouts(oldGeoPath)
        elif os.path.exists(self.geolayoutPath):
            self.read_geolayouts(self.geolayoutPath)

    def process_actor(self):
        """
        Process an actor by reading data, updating animation format, fixing animation data,
        fixing animations, re-exporting animations, and fixing geos.
        """
        # Check if a file with the DMA_EXPORT_PATH setting for this actor exists
        if os.path.exists(self.settings.get_actor_setting(self.name, "DMA_EXPORT_PATH")):
            return

        self.read_data()
        self.update_animation_format()
        self.fix_animation_data()
        self.fix_animations()
        self.reexport_animations()
        self.fix_geos()

    def __init__(self, name, animPath, geolayoutPath, settings):
        """
        Initialize an Actor object.

        Args:
            name (str): The name of the actor.
            animPath (str): The path to the animation files.
            geolayoutPath (str): The path to the geolayout files.
            settings (Settings): A Settings object containing the settings for the converter.
        """

        self.name = name

        self.animPath = animPath
        self.geolayoutPath = geolayoutPath
        
        self.animFilesArrays = dict()
        self.animFilesTexts = dict()

        self.oldAnimData = dict()
        self.newAnimData = dict()

        self.oldAnims = []
        self.newAnims = []

        self.geolayoutArrays = dict()
        self.geolayoutTexts = dict()
        self.geolayouts = []

        self.settings = settings

class ActorSettings:
    """
    A class to represent actor settings.
    """

    def __init__(self, generalSettings = dict(), animSettings = dict(), geoSettings = dict()):
        """
        Initialize an ActorSettings object.

        Args:
            generalSettings (dict): A dictionary of general settings.
            animSettings (dict): A dictionary of individual animation settings.
            geoSettings (dict): A dictionary of individual geolayout settings.
        """

        self.generalSettings = generalSettings
        self.animSettings = animSettings
        self.geoSettings = geoSettings

    def to_json_data(self):
        """
        Convert the ActorSettings object to JSON data.

        Returns:
            dict: A dictionary representing the ActorSettings object in JSON format.
        """

        return {
            'generalSettings': self.generalSettings,
            'animSettings': self.animSettings,
            'geoSettings': self.geoSettings,
        }

class Settings:
    """
    A class with the settings for the converter.
    """
    
    def __init__(self):
        """
        Initialize a new Settings object with default values.
        """
        self.searchableDirs = ["actors/"]

        self.globalSettings = {
            "ALWAYS_USE_DOT_OLD": True,

            # Format settings
            "USE_THREE_ELEMENT_QUATERNIONS": True,
            "USE_FIXED_EXPONENT_BFLOATS": True,
            
            # Bone translation options
            "TRANSLATE_BONE": "ANY", # ANY, FIRST, NONE
            "SCALE_BONE": "ANY", # ANY, FIRST, NONE

            # Translation flag options
            "APPLY_HORIZONTAL_TRANS_FLAG": True,
            "APPLY_VERTICAL_TRANS_FLAG": True,
            "APPLY_NO_TRANS_FLAG": True,
            "APPLY_TRANS_FLAGS": True,

            # Flags to remove
            "FLAGS_TO_REMOVE": [
                "ANIM_FLAG_HOR_TRANS", 
                "ANIM_FLAG_VERT_TRANS",
                "ANIM_FLAG_NO_TRANS"
            ],

            # Actor search options
            "SKIP": False,

            # DMA options
            "IS_DMA": False,
            "PER_FRAME_DMA": False,

            # Geolayout paths
            "GEOLAYOUT_PATH": "FIND_AUTO",
            "SPECIFIC_GEO": "FIND_AUTO",

            "CAN_MERGE_FIRST_BONE": False,
            "REMOVE_CHILDLESS_NULL_BONES": True,
        
            "ANIMATION_PATH": "FIND_AUTO",

            # DMA export options
            "DMA_EXPORT_PATH": "NONE",
            "DMA_ANIM_TABLE_NAME": "NONE",
            "DMA_ANIM_TABLE_STRUCT_NAME": "NONE",

            "MAX_BONE_DIFFERENCE": 0,

            "MAX_THREADS": 32,

            "VANILLA_ACTORS": [
                "amp",
                "bird",
                "blargg",
                "blue_coin_switch",
                "blue_fish",
                "bobomb",
                "bomb",
                "boo",
                "boo_castle",
                "book",
                "bookend",
                "bowling_ball",
                "bowser",
                "bowser_flame",
                "bowser_key",
                "breakable_box",
                "breath_meter",
                "bub",
                "bubba",
                "bubble",
                "bullet_bill",
                "bully",
                "burn_smoke",
                "butterfly",
                "cannon_barrel",
                "cannon_base",
                "cannon_lid",
                "capswitch",
                "castle_flag",
                "chain_ball",
                "chain_chomp",
                "chair",
                "checkerboard_platform",
                "chillychief",
                "chuckya",
                "clam_shell",
                "coin",
                "cyan_fish",
                "dirt",
                "door",
                "dorrie",
                "exclamation_box",
                "exclamation_box_outline",
                "explosion",
                "eyerok",
                "flame",
                "flyguy",
                "fwoosh",
                "goomba",
                "haunted_cage",
                "heart",
                "heave_ho",
                "hoot",
                "impact_ring",
                "impact_smoke",
                "king_bobomb",
                "klepto",
                "koopa",
                "koopa_flag",
                "koopa_shell",
                "lakitu_cameraman",
                "lakitu_enemy",
                "lantern",
                "leaves",
                "mad_piano",
                "manta",
                "mario",
                "mario_cap",
                "metal_box",
                "mips",
                "mist",
                "moneybag",
                "monty_mole",
                "monty_mole_hole",
                "mr_i_eyeball",
                "mr_i_iris",
                "mushroom_1up",
                "number",
                "peach",
                "pebble",
                "penguin",
                "piranha_plant",
                "pokey",
                "poundable_pole",
                "power_meter",
                "purple_switch",
                "sand",
                "scuttlebug",
                "seaweed",
                "skeeter",
                "small_key",
                "small_water_splash",
                "smoke",
                "snowman",
                "snufit",
                "sparkle",
                "sparkle_animation",
                "spindrift",
                "spiny",
                "spiny_egg",
                "springboard",
                "star",
                "sushi",
                "swoop",
                "test_platform",
                "thwomp",
                "toad",
                "tornado",
                "transparent_star",
                "treasure_chest",
                "tree",
                "ukiki",
                "unagi",
                "walk_smoke",
                "warp_pipe",
                "water_bubble",
                "water_mine",
                "water_ring",
                "water_splash",
                "water_wave",
                "whirlpool",
                "white_particle",
                "white_particle_small",
                "whomp",
                "wiggler",
                "wiggler_body",
                "wiggler_head",
                "wooden_signpost",
                "yellow_sphere",
                "yellow_sphere_small",
                "yoshi",
                "yoshi_egg"
            ],

            # Debug setitngs
            "FORCE_CLEAN": False,
            "UNI_TEST": False
        }

        self.vanillaActorSettings = {}

        self.actorSettings = {
            "amp": ActorSettings(
                animSettings = {
                    "dAmpAnimation": { 
                        "SPECIFIC_GEO": "dAmpGeo"
                    }
                }
            ),
            "bowser": ActorSettings(
                generalSettings = {
                    "SPECIFIC_GEO": "bowser_geo_0000D8"
                }
            ),
            "mario": ActorSettings(
                generalSettings = {
                    "CAN_MERGE_FIRST_BONE": True,
                    "IS_DMA": True,
                    "PER_FRAME_DMA": True,
                    "ANIMATION_PATH": "assets/anims",
                    "DMA_EXPORT_PATH": "build/us_n64/assets/mario_anim_data.c",
                    "DMA_ANIM_TABLE_NAME": "gMarioAnims",
                    "DMA_ANIM_TABLE_STRUCT_NAME": "MarioAnimsObj"
                }, 
                animSettings = {
                    "anim_09": { # Handstand idle
                        "APPLY_TRANS_FLAGS": False
                    },
                    "anim_0B": { # Start handstand
                        "APPLY_TRANS_FLAGS": False
                    },
                    "anim_0C": { # Return from handstand
                        "APPLY_TRANS_FLAGS": False
                    },
                    "anim_25": { # Final bowser raise hand spin
                        "APPLY_TRANS_FLAGS": False
                    },
                    "anim_26": { # Final bowser wing cap take off
                        "APPLY_TRANS_FLAGS": False
                    },
                    "anim_5F": { # Pull door walk in
                        "APPLY_TRANS_FLAGS": False
                    },
                    "anim_60": { # Push door walk in
                        "APPLY_TRANS_FLAGS": False
                    },
                    "anim_61": { # Unlock door
                        "APPLY_TRANS_FLAGS": False
                    },
                    "anim_9C": { # Summon star
                        "APPLY_TRANS_FLAGS": False
                    },
                    "anim_9D": { # Return star approach door
                        "APPLY_TRANS_FLAGS": False
                    }
                }
            )
        }

        self.specialGeos = {
            "castle_flag": ActorSettings(
                generalSettings = {
                    "GEOLAYOUT_PATH": "levels/castle_grounds/areas/1/11/geo.inc.c",
                    "ANIMATION_PATH": "levels/castle_grounds/areas/1/11/anim.inc.c"
                })
        }

    def set_global_setting(self, name:str, value):
        """
        Sets a global setting.

        Args:
            name (str): The name of the global setting to set.
            value: The value to set the global setting to.
        """
        self.globalSettings[name] = value

    def set_setting_to_actor(self, actorName:str, settingName:str, value):
        """
        Sets a setting for a specific actor.

        Args:
            actorName (str): The name of the actor to set the setting for.
            settingName (str): The name of the setting to set.
            value: The value to set the setting to.
        """
        if actorName not in self.actorSettings:
            self.actorSettings[actorName] = ActorSettings()

        self.actorSettings[actorName].generalSettings[settingName] = value

    def set_setting_to_anim(self, actorName:str, animName:str, settingName:str, value):
        """
        Sets a setting for a specific animation of an actor.

        Args:
            actorName (str): The name of the actor to set the setting for.
            animName (str): The name of the animation to set the setting for.
            settingName (str): The name of the setting to set.
            value: The value to set the setting to.
        """
        if actorName not in self.actorSettings:
            self.actorSettings[actorName] = ActorSettings()

        actorSettings = self.actorSettings[actorName]

        if animName not in actorSettings.animSettings:
            actorSettings.animSettings[animName] = dict()

        actorSettings.animSettings[animName][settingName] = value
    
    def set_setting_to_geo(self, actorName:str, geoName:str, settingName:str, value):
        """
        Sets a setting for a specific geolayout of an actor.

        Args:
            actorName (str): The name of the actor to set the setting for.
            geoName (str): The name of the geolayout to set the setting for.
            settingName (str): The name of the setting to set.
            value: The value to set the setting to.
        """
        if actorName not in self.actorSettings:
            self.actorSettings[actorName] = ActorSettings()

        actorSettings = self.actorSettings[actorName]

        if geoName not in actorSettings.geoSettings:
            actorSettings.geoSettings[geoName] = dict()

        actorSettings.geoSettings[geoName][settingName] = value

    def get_global_setting(self, settingName:str):
        """
        Gets a global setting.

        Args:
            settingName (str): The name of the global setting to get.

        Returns:
            The value of the global setting.
        """
        return self.globalSettings[settingName]

    def get_actor_setting(self, actorName:str, settingName:str):
        """
        Gets a setting for a specific actor.

        Args:
            actorName (str): The name of the actor to get the setting for.
            settingName (str): The name of the setting to get.

        Returns:
            The value of the actor/special geo's setting if it exists, otherwise returns the global setting.
        """

        # Check if the actor has any settings and if the specific setting exists
        if actorName in self.actorSettings and settingName in self.actorSettings[actorName].generalSettings:
            return self.actorSettings[actorName].generalSettings[settingName]
        elif actorName in self.specialGeos and settingName in self.specialGeos[actorName].generalSettings:
            return self.specialGeos[actorName].generalSettings[settingName]
        elif actorName in self.get_global_setting("VANILLA_ACTORS") and settingName in self.vanillaActorSettings:
            return self.vanillaActorSettings[settingName]
        else:
            return self.get_global_setting(settingName)
    
    def get_special_geo_setting(self, specialGeoName:str, settingName:str):
        """
        Gets a setting for a special geo.

        Args:
            specialGeoName (str): The name of the special geo to get the setting for.
            settingName (str): The name of the setting to get.

        Returns:
            The value of the actor's setting if it exists, otherwise returns the global setting.
        """
        # Check if the actor has any settings and if the specific setting exists
        if specialGeoName in self.specialGeos and settingName in self.specialGeos[specialGeoName].generalSettings:
            return self.specialGeos[specialGeoName].generalSettings[settingName]
        else:
            return self.get_global_setting(settingName)

    def get_anim_setting(self, actorName:str, animName:str, settingName:str):
        """
        Gets a setting for a specific animation of an actor.

        Args:
            actorName (str): The name of the actor to get the setting for.
            animName (str): The name of the animation to get the setting for.
            settingName (str): The name of the setting to get.

        Returns:
            The value of the animation's setting if it exists, otherwise returns the actor's setting if it exists,
            otherwise returns the global setting.
        """
        # Check if the actor has any settings
        if actorName in self.actorSettings:
            animsSettings = self.actorSettings[actorName].animSettings
            
            # Check if the animation has any settings
            if animName in animsSettings:
                individualAnimSettings = animsSettings[animName]

                if settingName in individualAnimSettings:
                    return individualAnimSettings[settingName]

        # If the specific animation setting does not exist, return the global setting´s value
        return self.get_actor_setting(actorName, settingName)

    def get_geo_setting(self, actorName:str, geoName:str, settingName:str):
        """
        Gets a setting for a specific geolayout of an actor.

        Args:
            actorName (str): The name of the actor to get the setting for.
            geoName (str): The name of the geolayout to get the setting for.
            settingName (str): The name of the setting to get.

        Returns:
            The value of the geolayout's setting if it exists, otherwise returns 
            the actor's setting if it exists, otherwise returns the global setting.
        """
        # Check if the actor has any settings
        if actorName in self.actorSettings:
            geoSettings = self.actorSettings[actorName].geoSettings
            
            # Check if the geolayout has any settings
            if geoName in geoSettings:
                individualGeoSettings = geoSettings[geoName]

                if settingName in individualGeoSettings:
                    return individualGeoSettings[settingName]

        # If the specific geolayout setting does not exist, return the global setting´s value
        return self.get_actor_setting(actorName, settingName)

    def to_json_data(self):
        """
        Convert object data to JSON format.

        Returns:
            dict: A dictionary containing the object's data in JSON format.
        """

        # Convert actor settings to JSON format
        actorSettings = [
            (actorName, self.actorSettings[actorName].to_json_data())
            for actorName in self.actorSettings.keys()
        ]
        actorSettings.sort()

        # Convert special geos settings to JSON format
        specialGeos = [
            (specialGeoName, self.specialGeos[specialGeoName].to_json_data())
            for specialGeoName in self.specialGeos.keys()
        ]
        specialGeos.sort()

        # Create a dictionary containing the settings's data
        data = {
            'globalSettings': self.globalSettings,
            'vanillaActorSettings': self.vanillaActorSettings,
            'actorSettings': actorSettings,
            'searchableDirs': self.searchableDirs,
            'specialGeos': specialGeos
        }
        
        return data

    def from_dict(self, data: dict):
        """
        Load settings data from a dictionary.

        Args:
            data (dict): A dictionary containing the settings's.
        """

        # Load global settings
        globalSettingsDict = data['globalSettings']
        
        # Set each global setting
        for settingKey in globalSettingsDict:
            self.set_global_setting(settingKey, globalSettingsDict[settingKey])
        
        self.vanillaActorSettings = data['vanillaActorSettings']

        # Load actor settings
        self.actorSetting = dict()
        
        # Set each actor setting
        for actor in data['actorSettings']:
            generalSettings = actor[1]['generalSettings']
            animSettings = actor[1]['animSettings']
            geoSettings = actor[1]['geoSettings']
            
            self.actorSettings[actor[0]] = ActorSettings(generalSettings, animSettings, geoSettings)

        # Load searchable directories
        self.searchableFolders = data['searchableDirs']

        # Load special geos settings
        self.specialGeos = dict()

        # Set each special geo setting
        for actor in data['specialGeos']:
            generalSettings = actor[1]['generalSettings']
            animSettings = actor[1]['animSettings']
            geoSettings = actor[1]['geoSettings']
            
            self.specialGeos[actor[0]] = ActorSettings(generalSettings, animSettings, geoSettings)

threadLimiter = None

def start_actor(name: str, animDir: str, geoPath: str, settings: Settings):
    global threadLimiter

    # Acquire the semaphore to limit the number of threads
    threadLimiter.acquire()

    actor = Actor(name, animDir, geoPath, settings)

    def start_actor_and_handle_thread():
        actor.process_actor()
        # Release the semaphore when the thread finishes
        threadLimiter.release()

    # Start new thread to run the process_actor function
    thread = threading.Thread(target=start_actor_and_handle_thread)
    thread.start()

def search_through_special_geos(settings):
    """
    Searches through special geos specified in the settings and processes each actor.

    Args:
        settings (Settings): A Settings object containing the settings for the converter.
    """
    for specialGeoKey in settings.specialGeos.keys():
        start_actor(specialGeoKey, 
                        settings.get_special_geo_setting(specialGeoKey, "ANIMATION_PATH"),
                        settings.get_special_geo_setting(specialGeoKey, "GEOLAYOUT_PATH"),
                        settings)

def process_actor_folder(name: str, dir: str, settings: Settings):
    """
    Processes an actor folder by retrieving the animation and geolayout paths from the settings object,
    generating the paths if set to FIND_AUTO.

    Args:
        name (str): The name of the actor.
        dir (str): The directory of the actor folder.
        settings (Settings): A Settings object containing the settings for the converter.
    """

    # Get animation and geolayout paths from settings
    actorSettingAnimPath = settings.get_actor_setting(name, "ANIMATION_PATH")
    actorSettingGeoPath = settings.get_actor_setting(name, "GEOLAYOUT_PATH")

    # Generate animation and geolayout paths based on directory
    animDir = f"{dir}/anims/"
    geoPath = f"{dir}/geo.inc.c"

    # If animation path is not set to FIND_AUTO, use path specified in settings
    if actorSettingAnimPath != "FIND_AUTO":
        animDir = actorSettingAnimPath

    # If geolayout path is not set to FIND_AUTO, use path specified in settings
    if actorSettingGeoPath != "FIND_AUTO":
        geoPath = actorSettingGeoPath

    start_actor(name, animDir, geoPath, settings)

def search_through_folders(settings: Settings):
    """
    Searches through folders specified in the settings for actor folders and processes them.

    Args:
        settings (Settings): A Settings object containing the settings for the converter.
    """

    # Iterate through the searchable folders
    for folder in settings.searchableDirs:
        # Iterate through the names in the folder
        for name in os.listdir(folder):
            # Check if the name should be skipped in the actor search
            if settings.get_actor_setting(name, "SKIP"):
                continue

            # Check if the name is a directory
            dir = os.path.join(folder, name)
            if os.path.isdir(dir):
                process_actor_folder(name, dir, settings)

def clean_file(filePath: str):
    """
    Cleans up a file by removing the '.old' extension if it exists.

    Args: 
        filePath (str): The path to the file to clean up.
    """
    # Check if the file path does not already end with '.old'
    if not filePath.endswith('.old'):
        # Construct the old and new file paths
        oldFilePath = f"{filePath}.old"
        newFilePath = filePath

        if os.path.exists(oldFilePath) and os.path.exists(newFilePath):
            # Remove the new file and rename the old file to the new file path
            os.remove(newFilePath)
            os.rename(oldFilePath, newFilePath)

def clean(directoryOrFilePath: str, deleteFile:bool = False):
    """
    Cleans up a directory or file by removing the '.old' extension from all files in the directory or from the file itself.

    Args: 
        directoryOrFilePath (str): The path to the directory or file to clean up.
        deleteFile (bool): If True, delete the file instead of cleaning it. Defaults to False.
    """

    if os.path.isfile(directoryOrFilePath):
        # If deleteFile is True, delete the file
        if deleteFile == True:
            os.remove(directoryOrFilePath)
        else:
            clean_file(directoryOrFilePath)

    # Check if the path is a folder
    elif os.path.isdir(directoryOrFilePath):
        # Iterate through all files in the directory and its subdirectories
        for root, dirs, files in os.walk(directoryOrFilePath):
            for file_name in files:
                file_path = os.path.join(root, file_name)
                clean_file(file_path)

def test_fixed_exponent_bfloats(start:float, end:float, increment:float, exponent:int):
    """
    Tests the conversion of floats to fixed exponent bfloats and back.

    Args:
        start (float): The starting value for the test range.
        end (float): The ending value for the test range.
        increment (float): The increment for the float_to_fixed_exponent_bfloat function.
        exponent (int): The exponent for the float_to_fixed_exponent_bfloat and fixed_exponent_bfloat_to_float functions.
    """
    diviser = abs(end - start) / 10.0

    print_test(f"\
\n\
\n\
\n\
Testing fixed exponent bfloats:\n\
start value: {start}; end value: {end}\n\
increment: {increment}; increment: {exponent}\n\
diviser: {diviser}\
\n\
")
    x = start
    while x <= end:
        # Convert the float to a fixed exponent bfloat
        bX = float_to_fixed_exponent_bfloat(x, increment, exponent)
        if bX > 32767:
            print_error("Float bellow is above s16 value")
        
        # Convert the fixed exponent bfloat back to a float
        resultX = fixed_exponent_bfloat_to_float(bX, exponent << 23, increment)
        
        # Calculate the difference between the original float and the result
        difference = abs(x - resultX)

        # Print the test results
        print_test(f"\
Original value - {x}\n\
Fixed exponent (128) bfloat - {bX} ({bin(bX)})\n\
Float from bfloat - {resultX}\n\
Difference - {difference}\n\
")
        x += diviser

def run_tests(settings: Settings):
    """
    Runs tests for fixed exponent bfloats.

    Args:
        settings (Settings): A Settings object containing the settings for the converter.
    """
    print_test("Beggining tests.")

    # Tests rotation values bfloats
    test_fixed_exponent_bfloats(-1.0,  1.0,  3.0,  128)

    # Tests scaling values bfloats
    test_fixed_exponent_bfloats(-16.0, 16.0, 48.0, 132)

def clean_all(settings: Settings):
    """
    Cleans up all directories and files specified in the settings object.
    
    Args: 
        settings (Settings): A Settings object containing the settings for the converter.
    """

    for directory in settings.searchableDirs:
        # Iterate through the names in the folder
        for name in os.listdir(directory):
            # Check if the name should be skipped in the actor search
            if settings.get_actor_setting(name, "SKIP"):
                continue

            actorDirectory = os.path.join(directory, name)
            clean(actorDirectory)

            geolayoutPath = settings.get_actor_setting(name, "GEOLAYOUT_PATH")
            animationPath = settings.get_actor_setting(name, "ANIMATION_PATH")
            dmaAnimPath = settings.get_actor_setting(name, "DMA_EXPORT_PATH")

            clean(geolayoutPath)
            clean(animationPath)
            clean(dmaAnimPath, True)

    for specialGeoKey in settings.specialGeos.keys():
        geolayoutPath = settings.get_actor_setting(specialGeoKey, "GEOLAYOUT_PATH")
        animationPath = settings.get_actor_setting(specialGeoKey, "ANIMATION_PATH")

        clean(geolayoutPath)
        clean(animationPath)

def create_new_json_settings(path: str):
    """
    Creates a JSON file containing the default settings.

    Args:
        path (str): The path to the JSON file to be created.
    """

    settings = Settings()

    # Write the settings to the JSON file
    with open(path, 'w') as file:
        json.dump(settings.to_json_data(), file, indent=4)

def read_json_settings():
    """
    Reads settings for the animation converter from a JSON file and returns a Settings object.

    If the JSON file does not exist, it is created using the create_json_settings function.

    Returns:
        Settings: A Settings object containing the settings for the converter.
    """
    path = "quaternion_anim_settings.json"

    # Create a settings file if one does not exist already
    if not os.path.exists(path):
        create_new_json_settings(path)

    settings = Settings()

    # Try to read the settings from the JSON file
    try:
        with open(path, 'r') as f:
            data = json.load(f)
            settings.from_dict(data)

    # If an error occurs while reading the JSON file, create a new settings file
    except:
        create_new_json_settings(path)

    # Write the settings to the JSON file (this is to fix settings files with errors)
    with open(path, 'w') as file:
        json.dump(settings.to_json_data(), file, indent=4)

    return settings

def start_converter(): 
    """
    This function starts the animation converter.
    """
    # Read settings from JSON file
    settings = read_json_settings()

    # Check if any command line arguments were passed
    langs = ""
    if len(sys.argv) > 1:
        langs = sys.argv[1]

    # Check if "clean" was passed as an argument
    clean = "clean" in langs

    forceClean = settings.get_global_setting("FORCE_CLEAN")

    # If clean or forceClean is True, clean all conversions by recovering .old files
    if clean or forceClean:
        clean_all(settings)

        # If clean is True, return from the function
        if clean:
            return
    
    # Check if the uni test setting is on.
    # If so run a few simple tests for slightly trickier math problems.
    if settings.get_global_setting("UNI_TEST"):
        run_tests(settings)

    global threadLimiter
    maxThreads = settings.get_global_setting("MAX_THREADS")
    threadLimiter = threading.Semaphore(maxThreads)

    print_info("Quaternion converter started.") 

    # Search through folders for animations to convert and 
    # geolayouts to fix.
    search_through_folders(settings)
    search_through_special_geos(settings)

    print_info("Quaternion converter finished.") 

start_converter()