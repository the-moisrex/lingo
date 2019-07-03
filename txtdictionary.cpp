#include "txtdictionary.h"
#include <algorithm>

bool txtDictionary::isSupported(QOnlineTranslator::Language,
                                QOnlineTranslator::Language) const noexcept {}

void txtDictionary::search(const QString& word) {
  setLoading(true);

  found = std::find_if(data.cbegin(), data.cend(), [&](auto const& a) {
    return a.names.end() != std::find(a.names.begin(), a.names.end(), word);
  });

  setLoading(false);
}
