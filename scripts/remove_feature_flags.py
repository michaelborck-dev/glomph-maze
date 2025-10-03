#!/usr/bin/env python3
"""
Remove feature flag conditionals from myman.c

Strategy:
1. USE_WIDEC_SUPPORT (always 0) - remove entire blocks including code
2. USE_ATTR (always 1) - keep code, remove #if/#endif wrappers
3. USE_COLOR (always 1) - keep code, remove #if/#endif wrappers
"""

import re
import sys

def remove_always_false_blocks(content, flag_name):
    """Remove blocks where flag is always false (dead code)"""
    lines = content.split('\n')
    result = []
    skip_depth = 0
    depth_stack = []
    
    i = 0
    while i < len(lines):
        line = lines[i]
        stripped = line.strip()
        
        # Match #if/#ifdef with the flag
        if_match = re.match(r'#\s*if\s+.*\b' + re.escape(flag_name) + r'\b', stripped)
        ifdef_match = re.match(r'#\s*ifdef\s+' + re.escape(flag_name) + r'\b', stripped)
        
        if if_match or ifdef_match:
            # Start skipping this block
            skip_depth += 1
            depth_stack.append(('if', i))
        elif stripped.startswith('#elif') and skip_depth > 0:
            # In a block we're skipping, check if elif has the flag
            if re.search(r'\b' + re.escape(flag_name) + r'\b', stripped):
                pass  # Keep skipping
            else:
                # elif without flag - might be the else case, keep skipping for now
                pass
        elif stripped.startswith('#else') and skip_depth > 0:
            # Reached else clause - this code should be kept
            # But we need to check if we're in the right nesting level
            if len(depth_stack) > 0 and depth_stack[-1][0] == 'if':
                # This else belongs to our flag check
                pass  # Don't output the #else line itself
        elif stripped.startswith('#endif') and skip_depth > 0:
            skip_depth -= 1
            if len(depth_stack) > 0:
                depth_stack.pop()
            # Don't output the #endif
        elif skip_depth == 0:
            # Not skipping, keep the line
            result.append(line)
        # else: skip_depth > 0, so skip this line
        
        i += 1
    
    return '\n'.join(result)


def remove_always_true_wrappers(content, flag_name):
    """Remove #if/#endif wrappers where flag is always true, keep inner code"""
    lines = content.split('\n')
    result = []
    skip_wrapper_depth = 0
    normal_depth = 0
    
    i = 0
    while i < len(lines):
        line = lines[i]
        stripped = line.strip()
        
        # Check if this is an #if with our flag
        if_match = re.match(r'#\s*if\s+.*\b' + re.escape(flag_name) + r'\b', stripped)
        ifdef_match = re.match(r'#\s*ifdef\s+' + re.escape(flag_name) + r'\b', stripped)
        
        if if_match or ifdef_match:
            # Check if it's a negation (! or ==0)
            if re.search(r'!\s*' + re.escape(flag_name), stripped) or \
               re.search(re.escape(flag_name) + r'\s*==\s*0', stripped):
                # Negated - this code won't run, skip block
                skip_wrapper_depth += 1
                result.append(line)  # Keep the preprocessor line for now
            else:
                # Positive check - always true, skip the #if line but keep code
                skip_wrapper_depth += 1
                # Don't append the #if line
        elif stripped.startswith('#elif') and skip_wrapper_depth > 0:
            # In our wrapper - skip the #elif too
            pass
        elif stripped.startswith('#else') and skip_wrapper_depth > 0:
            # Else clause of always-true block - this code never runs
            # Skip until #endif
            normal_depth = skip_wrapper_depth
            skip_wrapper_depth = -1  # Mark that we're in dead else
        elif stripped.startswith('#endif'):
            if skip_wrapper_depth > 0:
                skip_wrapper_depth -= 1
                # Don't append the #endif
            elif skip_wrapper_depth < 0:
                skip_wrapper_depth = normal_depth - 1
                # Don't append the #endif
            else:
                result.append(line)
        elif skip_wrapper_depth < 0:
            # In dead else clause, skip
            pass
        else:
            # Normal line or inside always-true block
            result.append(line)
        
        i += 1
    
    return '\n'.join(result)


def main():
    if len(sys.argv) != 4:
        print("Usage: remove_feature_flags.py <input_file> <output_file> <flag_name>")
        print("Example: remove_feature_flags.py myman.c myman_new.c USE_WIDEC_SUPPORT")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    flag_name = sys.argv[3]
    
    with open(input_file, 'r') as f:
        content = f.read()
    
    # Determine if flag is always true or always false
    # For now, assume USE_WIDEC_SUPPORT is false, others are true
    if flag_name == 'USE_WIDEC_SUPPORT':
        result = remove_always_false_blocks(content, flag_name)
    else:
        result = remove_always_true_wrappers(content, flag_name)
    
    with open(output_file, 'w') as f:
        f.write(result)
    
    # Report stats
    original_lines = len(content.split('\n'))
    new_lines = len(result.split('\n'))
    print(f"Removed {original_lines - new_lines} lines")
    print(f"Original: {original_lines} lines")
    print(f"New: {new_lines} lines")


if __name__ == '__main__':
    main()
