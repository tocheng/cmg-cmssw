#include "RecoBTag/FeatureTools/interface/ChargedCandidateConverter.h"


namespace btagbtvdeep {

  void packedCandidateToFeatures(const pat::PackedCandidate * c_pf,
				 const pat::Jet & jet,
				 const TrackInfoBuilder & track_info,
				 const float drminpfcandsv, const float jetR,
				 ChargedCandidateFeatures & c_pf_features,
				 const bool flip) {

    commonCandidateToFeatures(c_pf, jet, track_info, drminpfcandsv, jetR, c_pf_features, flip);

    c_pf_features.vtx_ass = c_pf->pvAssociationQuality();

    c_pf_features.puppiw = c_pf->puppiWeight();

    // if PackedCandidate does not have TrackDetails this gives an Exception
    // because unpackCovariance might be called for pseudoTrack/bestTrack
    if (c_pf->hasTrackDetails()) {
      const auto & pseudo_track =  c_pf->pseudoTrack();
      c_pf_features.chi2 = catch_infs_and_bound(pseudo_track.normalizedChi2(),300,-1,300);
      // this returns the quality enum not a mask.
      c_pf_features.quality = pseudo_track.qualityMask();
    } else {
      // default negative chi2 and loose track if notTrackDetails
      c_pf_features.chi2 = catch_infs_and_bound(-1,300,-1,300);
      c_pf_features.quality =(1 << reco::TrackBase::loose);
    }

  }

  void recoCandidateToFeatures(const reco::PFCandidate * c_pf,
			       const reco::Jet & jet,
			       const TrackInfoBuilder & track_info,
			       const float drminpfcandsv, const float jetR, const float puppiw,
			       const int pv_ass_quality,
			       const reco::VertexRef & pv,
			       ChargedCandidateFeatures & c_pf_features,
			       const bool flip) {

    commonCandidateToFeatures(c_pf, jet, track_info, drminpfcandsv, jetR, c_pf_features, flip);

    c_pf_features.vtx_ass = vtx_ass_from_pfcand(*c_pf, pv_ass_quality, pv);
    c_pf_features.puppiw = puppiw;

    const auto & pseudo_track =  (c_pf->bestTrack()) ? *c_pf->bestTrack() : reco::Track();
    c_pf_features.chi2 = catch_infs_and_bound(std::floor(pseudo_track.normalizedChi2()),300,-1,300);
    c_pf_features.quality = quality_from_pfcand(*c_pf);

  }

}

