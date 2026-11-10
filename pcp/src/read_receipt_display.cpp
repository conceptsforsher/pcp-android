#include "pcp/read_receipt_display.hpp"
#include <sstream>
namespace pcp {
std::string formatReadReceiptCount(int c) { if(c<=0)return"";if(c==1)return"Read by 1"; return"Read by "+std::to_string(c); }
std::string formatReadReceiptNames(const std::vector<std::string>& names, int max) { std::ostringstream os; for(size_t i=0;i<names.size()&&(int)i<max;i++){if(i>0)os<<", ";os<<names[i];} int r=(int)names.size()-max; if(r>0)os<<" and "<<r<<" others"; return os.str(); }
int getReadReceiptAvatarsLimit(int w) { return w/48; }
}
