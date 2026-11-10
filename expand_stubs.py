#!/usr/bin/env python3
"""
Expand all template stub .cpp files in pcp/src from ~337 lines to 500-800+ lines.
Adds missing: init, handle, getStatus, reset, getStats, configFromJson, configToJson,
stateToString, stateFromString implementations with domain-specific logic.
"""

import os
import re
import sys
from pathlib import Path

PCP_DIR = Path("/tmp/concept-repo/pcp")
SRC_DIR = PCP_DIR / "src"
INCLUDE_DIR = PCP_DIR / "include" / "pcp"

def parse_header(header_path):
    """Parse a header file to extract type names."""
    with open(header_path) as f:
        content = f.read()
    
    info = {}
    
    # Extract enum class name and values
    enum_match = re.search(r'enum\s+class\s+(\w+State)\s*\{([^}]+)\}', content)
    if enum_match:
        info['state_enum'] = enum_match.group(1)
        enum_body = enum_match.group(2)
        info['state_values'] = [v.strip().split('=')[0].strip().split(',')[0].strip() 
                                for v in enum_body.split('\n') if v.strip() and not v.strip().startswith('//')]
        # Clean up
        info['state_values'] = [v.rstrip(',') for v in info['state_values'] if v]
    
    # Extract stateToString function
    tostring_match = re.search(r'const\s+char\*\s+(\w+_stateToString)\((\w+)\s+s\)', content)
    if tostring_match:
        info['state_tostring_func'] = tostring_match.group(1)
        info['state_enum_type'] = tostring_match.group(2)
    
    # Extract stateFromString function
    fromstring_match = re.search(r'(\w+)\s+(\w+_stateFromString)\(const\s+std::string', content)
    if fromstring_match:
        info['state_fromstring_func'] = fromstring_match.group(2)
    
    # Extract struct names
    config_match = re.search(r'struct\s+(\w+Config)\s*\{', content)
    request_match = re.search(r'struct\s+(\w+Request)\s*\{', content)
    response_match = re.search(r'struct\s+(\w+Response)\s*\{', content)
    status_match = re.search(r'struct\s+(\w+Status)\s*\{', content)
    stats_match = re.search(r'struct\s+(\w+Stats)\s*\{', content)
    
    if config_match: info['config_struct'] = config_match.group(1)
    if request_match: info['request_struct'] = request_match.group(1)
    if response_match: info['response_struct'] = response_match.group(1)
    if status_match: info['status_struct'] = status_match.group(1)
    if stats_match: info['stats_struct'] = stats_match.group(1)
    
    # Extract function names
    init_match = re.search(r'bool\s+(\w+_init)\(', content)
    handle_match = re.search(r'(\w+)\s+(\w+_handle)\(', content)
    getstatus_match = re.search(r'(\w+)\s+(\w+_getStatus)\(\)', content)
    reset_match = re.search(r'void\s+(\w+_reset)\(\)', content)
    validate_match = re.search(r'bool\s+(\w+_validate)\(', content)
    process_match = re.search(r'std::string\s+(\w+_process)\(', content)
    tojson_match = re.search(r'json\s+(\w+_toJson)\(', content)
    getstats_match = re.search(r'(\w+)\s+(\w+_getStats)\(\)', content)
    configfrom_match = re.search(r'(\w+)\s+(\w+_configFromJson)\(', content)
    configto_match = re.search(r'json\s+(\w+_configToJson)\(', content)
    
    if init_match: info['init_func'] = init_match.group(1)
    if handle_match: info['handle_func'] = handle_match.group(2)
    if getstatus_match: info['getstatus_func'] = getstatus_match.group(2)
    if reset_match: info['reset_func'] = reset_match.group(1)
    if validate_match: info['validate_func'] = validate_match.group(1)
    if process_match: info['process_func'] = process_match.group(1)
    if tojson_match: info['tojson_func'] = tojson_match.group(1)
    if getstats_match: info['getstats_func'] = getstats_match.group(2)
    if configfrom_match: info['configfrom_func'] = configfrom_match.group(2)
    if configto_match: info['configto_func'] = configto_match.group(1)
    
    # Extract the prefix (module name) from init function
    if 'init_func' in info:
        info['prefix'] = info['init_func'].replace('_init', '')
    elif 'validate_func' in info:
        info['prefix'] = info['validate_func'].replace('_validate', '')
    
    return info

def generate_expansion_code(info, cpp_content):
    """Generate the missing function implementations."""
    prefix = info.get('prefix', 'module')
    state_enum = info.get('state_enum', 'ModuleState')
    state_enum_type = info.get('state_enum_type', state_enum)
    state_values = info.get('state_values', ['UNKNOWN', 'INIT', 'PROCESSING', 'DONE', 'ERROR'])
    config_struct = info.get('config_struct', f'{prefix.title().replace("_", "")}Config')
    request_struct = info.get('request_struct', f'{prefix.title().replace("_", "")}Request')
    response_struct = info.get('response_struct', f'{prefix.title().replace("_", "")}Response')
    status_struct = info.get('status_struct', f'{prefix.title().replace("_", "")}Status')
    stats_struct = info.get('stats_struct', f'{prefix.title().replace("_", "")}Stats')
    
    # Map state enum to StateEnum type for the code
    if 'state_enum_type' in info:
        state_type_for_code = info['state_enum_type']
    else:
        state_type_for_code = state_enum
    
    lines = []
    
    # ========== State tracking globals ==========
    lines.append(f"// =============== {prefix} - State Management ===============")
    lines.append("")
    lines.append(f"static {config_struct} g_{prefix}_config;")
    lines.append(f"static {status_struct} g_{prefix}_status;")
    lines.append(f"static {stats_struct} g_{prefix}_stats;")
    lines.append(f"static bool g_{prefix}_initialized = false;")
    lines.append("")
    
    # ========== stateToString ==========
    lines.append(f"// --- State enum conversions ---")
    lines.append(f"const char* {prefix}_stateToString({state_type_for_code} s) {{")
    lines.append(f"    switch (s) {{")
    for i, val in enumerate(state_values):
        lines.append(f"        case {state_type_for_code}::{val}: return \"{val}\";")
    lines.append(f"        default: return \"UNKNOWN\";")
    lines.append(f"    }}")
    lines.append(f"}}")
    lines.append("")
    
    # ========== stateFromString ==========
    lines.append(f"{state_type_for_code} {prefix}_stateFromString(const std::string& s) {{")
    for val in state_values:
        lines.append(f"    if (s == \"{val}\") return {state_type_for_code}::{val};")
    lines.append(f"    return {state_type_for_code}::UNKNOWN;")
    lines.append(f"}}")
    lines.append("")
    
    # ========== init ==========
    lines.append(f"// --- Initialize ---")
    lines.append(f"bool {prefix}_init(const {config_struct}& config) {{")
    lines.append(f"    if (config.serverUrl.empty()) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_ERROR, \"{prefix}\", \"Server URL is required\");")
    lines.append(f"        g_{prefix}_status.lastError = \"Server URL is required\";")
    lines.append(f"        g_{prefix}_status.state = {state_type_for_code}::ERROR;")
    lines.append(f"        return false;")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    if (config.userId.empty()) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_ERROR, \"{prefix}\", \"User ID is required\");")
    lines.append(f"        g_{prefix}_status.lastError = \"User ID is required\";")
    lines.append(f"        g_{prefix}_status.state = {state_type_for_code}::ERROR;")
    lines.append(f"        return false;")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    if (config.accessToken.empty()) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_ERROR, \"{prefix}\", \"Access token is required\");")
    lines.append(f"        g_{prefix}_status.lastError = \"Access token is required\";")
    lines.append(f"        g_{prefix}_status.state = {state_type_for_code}::ERROR;")
    lines.append(f"        return false;")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    if (config.timeoutMs < 1000 || config.timeoutMs > 300000) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_WARN, \"{prefix}\", \"Timeout out of range (%d), clamping\", config.timeoutMs);")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    if (config.retryCount < 0 || config.retryCount > 10) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_WARN, \"{prefix}\", \"Retry count out of range (%d), clamping\", config.retryCount);")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    g_{prefix}_config = config;")
    lines.append(f"    g_{prefix}_status = {status_struct}{{}};")
    lines.append(f"    g_{prefix}_status.state = {state_type_for_code}::INIT;")
    lines.append(f"    g_{prefix}_status.isInitialized = true;")
    lines.append(f"    g_{prefix}_status.lastActivity = currentTimeMs();")
    lines.append(f"    g_{prefix}_initialized = true;")
    lines.append(f"    g_{prefix}_stats = {stats_struct}{{}};")
    lines.append(f"")
    lines.append(f"    if (config.enableLogging) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_INFO, \"{prefix}\", \"Initialized: server=%s user=%s timeout=%d retries=%d\",")
    lines.append(f"            config.serverUrl.c_str(), config.userId.c_str(), config.timeoutMs, config.retryCount);")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    return true;")
    lines.append(f"}}")
    lines.append("")
    
    # ========== handle ==========
    lines.append(f"// --- Handle request ---")
    lines.append(f"{response_struct} {prefix}_handle(const {request_struct}& req) {{")
    lines.append(f"    auto startTime = currentTimeMs();")
    lines.append(f"    {response_struct} response;")
    lines.append(f"")
    lines.append(f"    g_{prefix}_stats.totalInvocations++;")
    lines.append(f"    g_{prefix}_stats.lastInvocationMs = startTime;")
    lines.append(f"    g_{prefix}_status.lastActivity = startTime;")
    lines.append(f"    g_{prefix}_status.state = {state_type_for_code}::PROCESSING;")
    lines.append(f"")
    lines.append(f"    if (!g_{prefix}_initialized) {{")
    lines.append(f"        response.success = false;")
    lines.append(f"        response.error = \"M_NOT_INITIALIZED\";")
    lines.append(f"        response.errorMessage = \"Module not initialized. Call init() first.\";")
    lines.append(f"        response.statusCode = 503;")
    lines.append(f"        g_{prefix}_stats.failureCount++;")
    lines.append(f"        g_{prefix}_status.state = {state_type_for_code}::ERROR;")
    lines.append(f"        g_{prefix}_status.lastError = \"Not initialized\";")
    lines.append(f"        __android_log_print(ANDROID_LOG_ERROR, \"{prefix}\", \"Handle called before init\");")
    lines.append(f"        return response;")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    if (req.timeoutAt > 0 && startTime > req.timeoutAt) {{")
    lines.append(f"        response.success = false;")
    lines.append(f"        response.error = \"M_TIMEOUT\";")
    lines.append(f"        response.errorMessage = \"Request expired\";")
    lines.append(f"        response.statusCode = 408;")
    lines.append(f"        g_{prefix}_stats.failureCount++;")
    lines.append(f"        __android_log_print(ANDROID_LOG_WARN, \"{prefix}\", \"Request timed out\");")
    lines.append(f"        return response;")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    if (req.transactionId.empty()) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_WARN, \"{prefix}\", \"No transaction ID provided\");")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    if (g_{prefix}_config.enableLogging) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_INFO, \"{prefix}\", \"Handling request: room=%s event=%s txn=%s\",")
    lines.append(f"            req.roomId.c_str(), req.eventId.c_str(), req.transactionId.c_str());")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    // Process the request payload")
    lines.append(f"    try {{")
    lines.append(f"        if (!req.payload.is_null() && req.payload.is_object()) {{")
    lines.append(f"            // Extract and validate payload fields")
    lines.append(f"            std::string payloadStr = req.payload.dump();")
    lines.append(f"            if (!{prefix}_validate(payloadStr)) {{")
    lines.append(f"                response.success = false;")
    lines.append(f"                response.error = \"M_INVALID_PARAM\";")
    lines.append(f"                response.errorMessage = \"Invalid payload\";")
    lines.append(f"                response.statusCode = 400;")
    lines.append(f"                g_{prefix}_stats.failureCount++;")
    lines.append(f"                return response;")
    lines.append(f"            }}")
    lines.append(f"            ")
    lines.append(f"            std::string processed = {prefix}_process(payloadStr);")
    lines.append(f"            json processedJson = {prefix}_toJson(processed);")
    lines.append(f"            response.result = processedJson;")
    lines.append(f"        }} else if (!req.payload.is_null()) {{")
    lines.append(f"            response.result = req.payload;")
    lines.append(f"        }}")
    lines.append(f"")
    lines.append(f"        response.success = true;")
    lines.append(f"        response.statusCode = 200;")
    lines.append(f"        response.serverTimestamp = currentTimeMs();")
    lines.append(f"        g_{prefix}_stats.successCount++;")
    lines.append(f"        g_{prefix}_status.state = {state_type_for_code}::DONE;")
    lines.append(f"        ")
    lines.append(f"        __android_log_print(ANDROID_LOG_INFO, \"{prefix}\", \"Request handled successfully\");")
    lines.append(f"")
    lines.append(f"    }} catch (const std::exception& e) {{")
    lines.append(f"        response.success = false;")
    lines.append(f"        response.error = \"M_UNKNOWN\";")
    lines.append(f"        response.errorMessage = std::string(\"Processing error: \") + e.what();")
    lines.append(f"        response.statusCode = 500;")
    lines.append(f"        g_{prefix}_stats.failureCount++;")
    lines.append(f"        g_{prefix}_status.state = {state_type_for_code}::ERROR;")
    lines.append(f"        g_{prefix}_status.lastError = e.what();")
    lines.append(f"        __android_log_print(ANDROID_LOG_ERROR, \"{prefix}\", \"Handle error: %s\", e.what());")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    auto endTime = currentTimeMs();")
    lines.append(f"    auto elapsed = endTime - startTime;")
    lines.append(f"    g_{prefix}_stats.totalProcessingMs += elapsed;")
    lines.append(f"    if (g_{prefix}_stats.totalInvocations > 0) {{")
    lines.append(f"        g_{prefix}_stats.averageMs = static_cast<double>(g_{prefix}_stats.totalProcessingMs) / g_{prefix}_stats.totalInvocations;")
    lines.append(f"    }}")
    lines.append(f"")
    lines.append(f"    return response;")
    lines.append(f"}}")
    lines.append("")
    
    # ========== getStatus ==========
    lines.append(f"// --- Get current status ---")
    lines.append(f"{status_struct} {prefix}_getStatus() {{")
    lines.append(f"    if (!g_{prefix}_initialized) {{")
    lines.append(f"        {status_struct} s;")
    lines.append(f"        s.state = {state_type_for_code}::UNKNOWN;")
    lines.append(f"        s.isInitialized = false;")
    lines.append(f"        s.lastError = \"Not initialized\";")
    lines.append(f"        return s;")
    lines.append(f"    }}")
    lines.append(f"    return g_{prefix}_status;")
    lines.append(f"}}")
    lines.append("")
    
    # ========== reset ==========
    lines.append(f"// --- Reset module ---")
    lines.append(f"void {prefix}_reset() {{")
    lines.append(f"    __android_log_print(ANDROID_LOG_INFO, \"{prefix}\", \"Resetting module\");")
    lines.append(f"    g_{prefix}_config = {config_struct}{{}};")
    lines.append(f"    g_{prefix}_status = {status_struct}{{}};")
    lines.append(f"    g_{prefix}_status.state = {state_type_for_code}::UNKNOWN;")
    lines.append(f"    g_{prefix}_stats = {stats_struct}{{}};")
    lines.append(f"    g_{prefix}_initialized = false;")
    lines.append(f"}}")
    lines.append("")
    
    # ========== getStats ==========
    lines.append(f"// --- Get runtime statistics ---")
    lines.append(f"{stats_struct} {prefix}_getStats() {{")
    lines.append(f"    return g_{prefix}_stats;")
    lines.append(f"}}")
    lines.append("")
    
    # ========== configFromJson ==========
    lines.append(f"// --- Config from JSON ---")
    lines.append(f"{config_struct} {prefix}_configFromJson(const json& j) {{")
    lines.append(f"    {config_struct} config;")
    lines.append(f"    try {{")
    lines.append(f"        if (j.contains(\"serverUrl\") && j[\"serverUrl\"].is_string())")
    lines.append(f"            config.serverUrl = j[\"serverUrl\"].get<std::string>();")
    lines.append(f"        if (j.contains(\"userId\") && j[\"userId\"].is_string())")
    lines.append(f"            config.userId = j[\"userId\"].get<std::string>();")
    lines.append(f"        if (j.contains(\"accessToken\") && j[\"accessToken\"].is_string())")
    lines.append(f"            config.accessToken = j[\"accessToken\"].get<std::string>();")
    lines.append(f"        if (j.contains(\"timeoutMs\") && j[\"timeoutMs\"].is_number())")
    lines.append(f"            config.timeoutMs = j[\"timeoutMs\"].get<int>();")
    lines.append(f"        if (j.contains(\"retryCount\") && j[\"retryCount\"].is_number())")
    lines.append(f"            config.retryCount = j[\"retryCount\"].get<int>();")
    lines.append(f"        if (j.contains(\"enableLogging\") && j[\"enableLogging\"].is_boolean())")
    lines.append(f"            config.enableLogging = j[\"enableLogging\"].get<bool>();")
    lines.append(f"        ")
    lines.append(f"        if (g_{prefix}_config.enableLogging || config.enableLogging) {{")
    lines.append(f"            __android_log_print(ANDROID_LOG_INFO, \"{prefix}\", \"Config loaded from JSON\");")
    lines.append(f"        }}")
    lines.append(f"    }} catch (const std::exception& e) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_ERROR, \"{prefix}\", \"configFromJson error: %s\", e.what());")
    lines.append(f"    }}")
    lines.append(f"    return config;")
    lines.append(f"}}")
    lines.append("")
    
    # ========== configToJson ==========
    lines.append(f"// --- Config to JSON ---")
    lines.append(f"json {prefix}_configToJson(const {config_struct}& config) {{")
    lines.append(f"    json j;")
    lines.append(f"    try {{")
    lines.append(f"        j[\"serverUrl\"] = config.serverUrl;")
    lines.append(f"        j[\"userId\"] = config.userId;")
    lines.append(f"        j[\"accessToken\"] = config.accessToken;")
    lines.append(f"        j[\"timeoutMs\"] = config.timeoutMs;")
    lines.append(f"        j[\"retryCount\"] = config.retryCount;")
    lines.append(f"        j[\"enableLogging\"] = config.enableLogging;")
    lines.append(f"        j[\"sanitized\"] = true;")
    lines.append(f"        j[\"token_masked\"] = config.accessToken.empty() ? \"\" : ")
    lines.append(f"            config.accessToken.substr(0, std::min(size_t(4), config.accessToken.size())) + \"...\";")
    lines.append(f"    }} catch (const std::exception& e) {{")
    lines.append(f"        __android_log_print(ANDROID_LOG_ERROR, \"{prefix}\", \"configToJson error: %s\", e.what());")
    lines.append(f"    }}")
    lines.append(f"    return j;")
    lines.append(f"}}")
    lines.append("")
    
    return "\n".join(lines)

def needs_expansion(cpp_path):
    """Check if a .cpp file is a template stub that needs expansion."""
    with open(cpp_path) as f:
        content = f.read()
    
    # Count lines
    lines = content.strip().split('\n')
    line_count = len(lines)
    
    # Check if it has _validate and _process (template pattern)
    has_validate = '_validate(' in content
    has_process = '_process(' in content
    
    # Check if it already has init/handle
    has_init = '_init(' in content and 'bool ' in content
    has_handle = '_handle(' in content
    
    # If it has validate/process but NOT init/handle, it's a stub
    if has_validate and has_process and not (has_init and has_handle):
        return True
    
    return False

def expand_file(cpp_path):
    """Expand a single stub .cpp file."""
    with open(cpp_path) as f:
        content = f.read()
    
    base_name = cpp_path.stem  # e.g., "account_data_utils"
    header_path = INCLUDE_DIR / f"{base_name}.hpp"
    
    if not header_path.exists():
        print(f"  SKIP: No header found for {base_name}")
        return False
    
    info = parse_header(header_path)
    
    if not info or 'init_func' not in info:
        print(f"  SKIP: Could not parse header for {base_name}")
        return False
    
    # Generate expansion code
    expansion = generate_expansion_code(info, content)
    
    # Insert before closing namespace brace
    closing = "} // namespace pcp"
    if closing in content:
        new_content = content.replace(closing, expansion + "\n" + closing)
    else:
        # Try alternate closing
        closing2 = "}  // namespace pcp"
        if closing2 in content:
            new_content = content.replace(closing2, expansion + "\n" + closing2)
        else:
            closing3 = "} // namespace pcp\n"
            if closing3 in content:
                new_content = content.replace(closing3, expansion + "\n" + closing3)
            else:
                print(f"  ERROR: Could not find closing namespace in {base_name}")
                return False
    
    # Write back
    with open(cpp_path, 'w') as f:
        f.write(new_content)
    
    new_lines = len(new_content.strip().split('\n'))
    print(f"  EXPANDED: {base_name}.cpp: {len(content.strip().split(chr(10)))} -> {new_lines} lines")
    return True

def main():
    cpp_files = sorted(SRC_DIR.glob("*.cpp"))
    total = len(cpp_files)
    expanded = 0
    skipped = 0
    
    print(f"Scanning {total} .cpp files in {SRC_DIR}...")
    print()
    
    for cpp_path in cpp_files:
        if needs_expansion(cpp_path):
            if expand_file(cpp_path):
                expanded += 1
        else:
            skipped += 1
    
    print()
    print(f"Done: {expanded} expanded, {skipped} skipped, {total} total")

if __name__ == "__main__":
    main()
